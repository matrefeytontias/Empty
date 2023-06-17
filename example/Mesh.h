#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Empty/math/vec.h"
#include "Empty/gl/Buffer.h"
#include "Empty/gl/VertexArray.h"
#include "Empty/gl/VertexStructure.h"
#include "Empty/utils/macros.h"
#include "Empty/utils/utils.hpp"


using namespace Empty;
using namespace Empty::gl;

/**
* Structure to load vertices and faces from a OFF or OBJ file.
*/
struct Mesh
{
	std::vector<math::vec3> vertices;
	/**
	 * Attribute loaded in an OFF file.
	 */
	std::vector<math::ivec3> faces;
	/**
	 * Attributes loaded in an OBJ file.
	 */
	std::vector<math::vec2> textureCoords;
	std::vector<math::vec3> normals;

	VertexArray vao;
	Buffer vertexBuffer;
	Buffer triBuffer;
	VertexStructure vStruct;

	Mesh(const std::string& label) : vao(label + "VAO"), vertexBuffer(label + "VBO"), triBuffer(label + "IBO") { }

	inline bool isIndexed() const
	{
		return !faces.empty();
	}

	bool load(const std::string& path)
	{
		std::ifstream file(path);
		if (file.fail())
		{
			TRACE("Cannot open file: " << path);
			return false;
		}

		if (utils::endsWith(path, ".off"))
			return loadOff(file);
		
		if (utils::endsWith(path, ".obj"))
			return loadObj(file);
		
		TRACE("Wrong file extension: must be .off or .obj");
		return false;
	}

private:
	void loadVertexBuffer(BufferUsage usage)
	{
		vStruct = VertexStructure(vertices.size());
		vStruct.add("aPosition", VertexAttribType::Float, 3);

		if (!textureCoords.empty())
			vStruct.add("aTexCoords", VertexAttribType::Float, 2);

		if (!normals.empty())
			vStruct.add("aNormal", VertexAttribType::Float, 3);

		vertexBuffer.setStorage(vStruct.bytesPerVertex() * vertices.size(), usage, nullptr);

		size_t offset = 0;
		size_t size;

		auto upload = [&](const auto& data)
		{
			if (!data.empty())
			{
				size = data.size() * sizeof(data[0]);
				vertexBuffer.uploadData(offset, size, data.data());
				offset += size;
			}
		};

		upload(vertices);
		upload(textureCoords);
		upload(normals);
	}

	void loadElementBuffer(BufferUsage usage)
	{
		triBuffer.setStorage(sizeof(math::ivec3) * faces.size(), usage, faces.data());
	}
	
	bool loadObj(std::ifstream& file)
	{
		std::vector<math::vec3> tempVertices;
		std::vector<math::vec2> tempTextCoords;
		std::vector<math::vec3> tempNormals;

		vertices.clear();
		faces.clear();
		textureCoords.clear();
		normals.clear();

		auto ignoreComments = [&]()
		{
			while (file.peek() == '#')
				file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		};

		auto nextLine = [&]()
		{
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		};

		ignoreComments();
		std::string firstWord;
		while (file.good())
		{
			file >> firstWord;
			if (firstWord == "mtllib")
			{
				// Load MTL file, ignore here
			}
			else if (firstWord == "usemtl")
			{
				// Use the mtl name for the next lines, ignore here
			}
			else if (firstWord == "o")
			{
				// Object, ignore here
			}
			else if (firstWord == "g")
			{
				// Group, ignore here
			}
			else if (firstWord == "s")
			{
				// Smooth shading, ignore here
			}
			else if (firstWord == "v")
			{
				// Vertex
				math::vec3 vertex;
				file >> vertex.x >> vertex.y >> vertex.z;
				if (file.fail())
				{
					TRACE("Invalid vertex");
					return false;
				}
				tempVertices.push_back(vertex);
			}
			else if (firstWord == "vt")
			{
				// Texture coordinates
				math::vec2 textCoord;
				file >> textCoord.x >> textCoord.y;
				if (file.fail())
				{
					TRACE("Invalid texture coordinates");
					return false;
				}
				tempTextCoords.push_back(textCoord);
			}
			else if (firstWord == "vn")
			{
				// Normal
				math::vec3 normal;
				file >> normal.x >> normal.y >> normal.z;
				if (file.fail())
				{
					TRACE("Invalid normal");
					return false;
				}
				tempNormals.push_back(normal);
			}
			else if (firstWord == "vp")
			{
				// Vertex parameter, ignore here
			}
			else if (firstWord == "f")
			{
				// Face: vertices/texture/normal indices
				std::vector<std::string> points(3);
				file >> points[0] >> points[1] >> points[2];
				if (file.fail())
				{
					TRACE("Invalid face");
					return false;
				}

				auto upload = [&] (auto& vector, auto& src, int i) {
					if (i > -1)
						vector.push_back(src[i]);
					else
						FATAL("Invalid index in OBJ file");
				};

				for (int i = 0; i < 3; i++)
				{
					std::vector<std::string> attribs = utils::split(points[i], "/");
					auto nbAttribs = attribs.size();

					int indices[3] = {-1, -1, -1};
					for (int j = 0; j < nbAttribs; j++)
						indices[j] = attribs[j] == "" ? -1 : std::stoi(attribs[j]) - 1;

					upload(vertices, tempVertices, indices[0]);
					upload(textureCoords, tempTextCoords, indices[1]);
					upload(normals, tempNormals, indices[2]);
				}
			}
			else if (firstWord == "l")
			{
				// Line, ignore here
			}
			else
			{
				TRACE("Invalid line");
				return false;
			}
			nextLine();
			ignoreComments();
		}

		if (file.fail())
		{
			TRACE("There was some error in the file");
			return false;
		}

		if (!textureCoords.empty() && textureCoords.size() != vertices.size())
		{
			TRACE("Different number of vertices and texture coordinates.");
			return false;
		}
		if (!normals.empty() && normals.size() != vertices.size())
		{
			TRACE("Different number of vertices and normals.");
			return false;
		}

		loadVertexBuffer(BufferUsage::StaticDraw);

		return true;
	}

	bool loadOff(std::ifstream& file)
	{
		std::vector<math::vec3> newVertices;
		std::vector<math::ivec3> newFaces;

		auto ignoreComments = [&]()
		{
			while (file.peek() == '#')
				file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		};

		auto nextLine = [&]()
		{
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		};

		std::string line(3, 0);
		ignoreComments();
		file.read(line.data(), 3);
		if (line != "OFF")
		{
			TRACE("Invalid header: file must start with 'OFF'");
			return false;
		}
		nextLine();

		ignoreComments();
		int numberOfVertices, numberOfFaces;
		file >> numberOfVertices >> numberOfFaces;
		nextLine();

		ignoreComments();
		math::vec3 vertex;
		for (int i = 0; i < numberOfVertices; i++)
		{
			file >> vertex.x >> vertex.y >> vertex.z;
			if (!file.good()) 
			{
				TRACE("Invalid vertex");
				return false;
			}

			newVertices.push_back(vertex);
			nextLine();
			ignoreComments();
		}

		math::ivec3 face;
		int n;
		for (int j = 0; j < numberOfFaces; j++)
		{
			file >> n;
			if (n != 3) 
			{
				TRACE("Invalid face format: faces must be triangles");
				return false;
			}
			file >> face.x >> face.y >> face.z;
			if (!file.good())
			{
				TRACE("Invalid face");
				return false;
			}
			newFaces.push_back(face);
			nextLine();
			ignoreComments();
		}

		vertices = newVertices;
		faces = newFaces;
		textureCoords.clear();
		normals.clear();

		loadVertexBuffer(BufferUsage::StaticDraw);
		loadElementBuffer(BufferUsage::StaticDraw);

		return true;
	}
};
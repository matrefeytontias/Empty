#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Empty/math/Vector.hpp"
#include "Empty/render/gl/Buffer.h"
#include "Empty/utils/macros.h"

using namespace render::gl;

static bool endsWith(const std::string& str, const std::string& suffix)
{
	return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

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

		if (endsWith(path, ".off"))
			return loadOff(file);
		
		if (endsWith(path, ".obj"))
			return loadObj(file);
		
		TRACE("Wrong file extension: must be .off or .obj");
		return false;
	}

private:
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

				for (int i = 0; i < 3; i++)
				{
					std::vector<std::string> attribs = utils::split(points[i], "/");

					vertices.push_back(tempVertices[std::stoi(attribs[0]) - 1]);

					if (attribs.size() == 2)
						textureCoords.push_back(tempTextCoords[std::stoi(attribs[1]) - 1]);

					if (attribs.size() == 3)
					{
						if (attribs[1] != "")
						{
							textureCoords.push_back(tempTextCoords[std::stoi(attribs[1]) - 1]);
						}
						normals.push_back(tempNormals[std::stoi(attribs[2]) - 1]);
					}
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

		return true;
	}
};
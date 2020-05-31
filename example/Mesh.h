#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "math/Vector.hpp"
#include "utils/macros.h"

/**
* Structure to load vertices and faces from a OFF file.
*/
struct Mesh
{
	std::vector<math::vec3> vertices;
	std::vector<math::ivec3> faces;

	bool load(const std::string& path)
	{
		std::vector<math::vec3> newVertices;
		std::vector<math::ivec3> newFaces;

		std::ifstream file(path);
		if (file.fail())
		{
			TRACE("Cannot open file: " << path);
			return false;
		}

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
		return true;
	}
};
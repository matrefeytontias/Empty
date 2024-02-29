#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "glad/glad.h"
#include "Empty/math/mat.h"

#ifdef WIN32
#undef near
#undef far
#endif

namespace Empty::gl
{
	enum struct CubemapFace : GLenum;
}

namespace Empty::utils
{
	/**
	 * Tells if a value is contained in a set of test values.
	 * Only guaranteed to work when all values are of the same type.
	 */
	template <typename T, typename... U>
	constexpr bool isOneOf(T val, T test, U... others)
	{
		return val == test || isOneOf(val, others...);
	}

	/**
	 * Base case of telling if a value is contained in a set of values :
	 * tell if two values are equal.
	 */
	template <typename T>
	constexpr bool isOneOf(T val, T test)
	{
		return val == test;
	}

	/**
	 * Sets the working directory.
	 * @param   argv    the program's argv, retrieved through the main function
	 */
	void setwd(char** argv);
	/**
	 * Returns the contents of a file as a string. Raises a runtime_error exception
	 * upon failure.
	 */
	std::string getFileContents(const std::string_view& path);
	/**
	 * Splits a string on delimiting characters. The delimiting string is a disjunction,
	 * meaning the input string is split by either of the characters in the delimiter string.
	 */
	std::vector<std::string_view> split(const std::string_view& s, const std::string_view& delim);

	/**
	 * Prints all available OpenGL error codes.
	 */
	void checkGLerror(const char* file, int line);

#ifndef min
	/**
	 * Minimum of two values of the same comparable type.
	 */
	template <typename T> inline T min(T a, T b)
	{
		return a < b ? a : b;
	}
#endif

#ifndef max
	/**
	 * Maximum of two values of the same comparable type.
	 */
	template <typename T> inline T max(T a, T b)
	{
		return a > b ? a : b;
	}
#endif

	template <typename T>
	inline T select(T val, bool s)
	{
		return s ? val : static_cast<T>(0);
	}

	/**
	 * Constructs a 4x4 3D perspective matrix in the given matrix object based on FOV,
	 * aspect ratio, near plane and far plane.
	 */
	void perspective(math::mat4& p, float fov, float ratio, float near, float far);
	/**
	 * Shorthand function to modify the aspect ratio of a perspective matrix.
	 */
	void setAspectRatio(math::mat4& p, float ratio);

	/**
	 * Extract the underlying value of a scoped enum value
	 * From https://stackoverflow.com/a/14589519/5446815
	 */
	template <typename Enum>
	constexpr auto value(Enum e) -> typename std::underlying_type_t<Enum>
	{
		return static_cast<typename std::underlying_type<Enum>::type>(e);
	}

	/**
	 * Utility function to get the enum value corresponding to a cubemap face index.
	 */
	Empty::gl::CubemapFace getCubeMapFaceFromIndex(int i);

	/**
	 * Returns whether a string finishes with a given suffix.
	 */
	bool endsWith(const std::string_view& str, const std::string_view& suffix);
}

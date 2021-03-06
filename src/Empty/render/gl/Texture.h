#pragma once

#include <type_traits>

#include "Empty/glad/glad.h"
#include "Empty/render/gl/GLEnums.hpp"
#include "Empty/render/gl/GLObject.h"
#include "Empty/utils/macros.h"

namespace render::gl
{
	/**
	 * Returns the dimensionality of a texture with a given texture target.
	 */
	constexpr int dimensionsFromTarget(TextureTarget target)
	{
		if (target == TextureTarget::Dynamic)
			return 0;
		else if (target == TextureTarget::Texture1D
				 || target == TextureTarget::Proxy1D
				 || target == TextureTarget::TextureBuffer)
			return 1;
		else if (target == TextureTarget::Texture3D
				 || target == TextureTarget::Texture2DArray
				 || target == TextureTarget::Texture2DMultisampleArray
				 || target == TextureTarget::TextureCubeMapArray
				 || target == TextureTarget::Proxy3D
				 || target == TextureTarget::Proxy2DArray
				 || target == TextureTarget::Proxy2DMultisampleArray
				 || target == TextureTarget::ProxyCubeMapArray)
			return 3;
		else
			return 2;
	}

	/**
	 * Returns whether a given texture target references a cubemap texture target.
	 */
	constexpr bool isTargetCubeMap(TextureTarget target)
	{
		return target == TextureTarget::TextureCubeMap || target == TextureTarget::ProxyCubeMap;
	}

	/**
	 * Returns the index of a cubemap face.
	 */
	constexpr int cubemapFaceIndex(CubeMapFace face)
	{
		return static_cast<int>(utils::value(face)) - static_cast<int>(utils::value(CubeMapFace::PlusX));
	}

	/**
	 * This struct holds everything one needs to bind a texture. Useful in case
	 * one wants to bind the texture without having access to the entire Texture object.
	 */
	struct TextureBinding
	{
		TextureBinding() = default;
		TextureBinding(TextureTarget target, std::shared_ptr<TextureId> id)
			: _target(target), _id(id)
		{}

		TextureTarget target() const { return _target; }
		std::shared_ptr<TextureId> id() const { return _id; }

		inline void bind(int unit) const
		{
			ASSERT(_target != TextureTarget::Dynamic && _id && "Invalid texture binding");
			glBindTextureUnit(unit, *_id);
		}
		inline void unbind(int unit) const
		{
			ASSERT(_target != TextureTarget::Dynamic && "Invalid texture binding");
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(utils::value(_target), 0);
		}
	private:
		TextureTarget _target = TextureTarget::Dynamic;
		std::shared_ptr<TextureId> _id;
	};

	/**
	 * General-purpose texture class. Does not map one-to-one with OpenGL texture objects, but holds a shared reference
	 * to one. It is possible to instantiate textures with targets and formats both known and unknown at compile-time.
	 * All textures are of immutable format ; while their content may be changed at will, their dimension and storage
	 * requirements may only be set once.
	 */
	template <TextureTarget CTTarget = TextureTarget::Dynamic, TextureFormat CTFormat = TextureFormat::Dynamic>
	struct Texture : public GLObject<TextureId>
	{
#define COPY_CTPARAMS TextureTarget t = CTTarget, TextureFormat f = CTFormat

		template <COPY_CTPARAMS,
			std::enable_if_t<t == TextureTarget::Dynamic && f == TextureFormat::Dynamic, int> = 0>
			Texture(TextureTarget target, TextureFormat format) : GLObject(target), _target(target), _format(format) {}

		template <COPY_CTPARAMS,
			std::enable_if_t<t != TextureTarget::Dynamic && f == TextureFormat::Dynamic, int> = 0>
			Texture(TextureFormat format) : GLObject(t), _target(t), _format(format) {}

		template <COPY_CTPARAMS,
			std::enable_if_t<t == TextureTarget::Dynamic && f != TextureFormat::Dynamic, int> = 0>
			Texture(TextureTarget target) : GLObject(target), _target(target), _format(f) {}

		template <COPY_CTPARAMS,
			std::enable_if_t<t != TextureTarget::Dynamic && f != TextureFormat::Dynamic, int> = 0>
			Texture() : GLObject(t), _target(t), _format(f) {}
#undef COPY_CTPARAMS
		
		/**
		 * Binds the texture to a given texture unit. This is necessary for
		 * drawing operations to access the texture.
		 */
		inline void bind(int unit) const
		{
			glBindTextureUnit(unit, *_id);
		}

		/**
		 * Unbinds the texture from a given texture unit.
		 */
		inline void unbind(int unit) const
		{
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(utils::value(_target), 0);
		}

#define COPY_CTPARAMS TextureTarget t = CTTarget

		/**
		 * Allocate storage for a 1-dimensional texture.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<dimensionsFromTarget(t) == 1, int> = 0>
			inline void setStorage(int levels, int width)
		{
			ASSERT(!_requirementsSet && "Storage requirements were already set for this Texture");
			glTextureStorage1D(*_id, levels, utils::value(_format), width);
			_requirementsSet = true;
		}

		/**
		 * Allocate storage for a 2-dimensional texture that is not a cubemap.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<dimensionsFromTarget(t) == 2 && !isTargetCubeMap(t), int> = 0>
			inline void setStorage(int levels, int width, int height)
		{
			ASSERT(!_requirementsSet && "Storage requirements were already set for this Texture");
			glTextureStorage2D(*_id, levels, utils::value(_format), width, height);
			_requirementsSet = true;
		}

		/**
		 * Allocate storage for a 3-dimensional texture.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<dimensionsFromTarget(t) == 3, int> = 0>
			inline void setStorage(int levels, int width, int height, int depth)
		{
			ASSERT(!_requirementsSet && "Storage requirements were already set for this Texture");
			glTextureStorage3D(*_id, levels, utils::value(_format), width, height, depth);
			_requirementsSet = true;
		}

		/**
		 * Allocate storage for a texture with target unknown at compile-time. Parameters
		 * that are not relevant to the actual texture target are ignored.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<t == TextureTarget::Dynamic, int> = 0>
			void setStorage(int levels, int width, int height = 1, int depth = 1)
		{
			ASSERT(!_requirementsSet && "Storage requirements were already set for this Texture");

			int dims = dimensionsFromTarget(_target);
			if (dims == 1)
				glTextureStorage1D(*_id, levels, utils::value(_format), width);
			else if (dims == 2)
				glTextureStorage2D(*_id, levels, utils::value(_format), width, height);
			else // if(dims == 3)
				glTextureStorage3D(*_id, levels, utils::value(_format), width, height, depth);

			_requirementsSet = true;
		}

		/**
		 * Upload data to a 1-dimensional texture.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<dimensionsFromTarget(t) == 1, int> = 0>
			inline void uploadData(int level, int x, int w, PixelFormat format, PixelType type, const void* data) const
		{
			glTextureSubImage1D(*_id, level, x, w, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a 2-dimensional texture that is not a cubemap.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<dimensionsFromTarget(t) == 2 && !isTargetCubeMap(t), int> = 0>
			inline void uploadData(int level, int x, int y, int w, int h, PixelFormat format, PixelType type, const void* data) const
		{
			glTextureSubImage2D(*_id, level, x, y, w, h, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a 3-dimensional texture.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<dimensionsFromTarget(t) == 3, int> = 0>
			inline void uploadData(int level, int x, int y, int z, int w, int h, int d, PixelFormat format, PixelType type, const void* data) const
		{
			glTextureSubImage3D(*_id, level, x, y, z, w, h, d, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a cubemap texture.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<isTargetCubeMap(t), int> = 0>
			inline void uploadData(CubeMapFace face, int level, int x, int y, int w, int h, PixelFormat format, PixelType type, const void* data) const
		{
			glTextureSubImage3D(*_id, level, x, y, cubemapFaceIndex(face), w, h, 1, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a non-cubemap texture with target unknown at compile-time. Parameters
		 * that are not relevant to the actual texture target are ignored.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<t == TextureTarget::Dynamic, int> = 0>
			void uploadData(int level, int x, int y, int z, int w, int h, int d, PixelFormat format, PixelType type, const void* data) const
		{
			ASSERT(!isTargetCubeMap(_target) && "cannot upload data directly to a cubemap ; use dedicated method instead");

			int dims = dimensionsFromTarget(_target);
			if (dims == 1)
				glTextureSubImage1D(*_id, level, x, w, utils::value(format), utils::value(type), data);
			else if (dims == 2)
				glTextureSubImage2D(*_id, level, x, y, w, h, utils::value(format), utils::value(type), data);
			else // if(dims == 3)
				glTextureSubImage3D(*_id, level, x, y, z, w, h, d, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a cubemap texture with target unknown at compile-time.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<t == TextureTarget::Dynamic, int> = 0>
			inline void uploadData(CubeMapFace face, int level, int x, int y, int w, int h, PixelFormat format, PixelType type, const void* data) const
		{
			ASSERT(isTargetCubeMap(_target) && "can only upload data to a cubemap ; use general-purpose method instead");

			glTextureSubImage3D(*_id, level, x, y, cubemapFaceIndex(face), w, h, 1, utils::value(format), utils::value(type), data);
		}

#undef COPY_CTPARAMS

		/**
		 * Gets value of integer-valued texture parameters.
		 */
		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::BaseLevel || CTParam == TextureParam::MaxLevel
			|| CTParam == TextureParam::ViewMinLayer || CTParam == TextureParam::ViewNumLayers
			|| CTParam == TextureParam::ViewMinLevel || CTParam == TextureParam::ViewNumLevels
			|| CTParam == TextureParam::ViewImmutableLevels || CTParam == TextureParam::HasImmutableFormat
			, int> = 0>
			int getParameter() const
		{
			int p;
			glGetTextureParameteriv(*_id, utils::value(CTParam), &p);
			return p;
		}

		/**
		 * Gets value of symbolic-valued texture parameter.
		 */
		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::DepthStencilMode
			|| CTParam == TextureParam::CompareFunc || CTParam == TextureParam::CompareMode
			|| CTParam == TextureParam::MinFilter || CTParam == TextureParam::MagFilter
			|| CTParam == TextureParam::SwizzleR || CTParam == TextureParam::SwizzleG
			|| CTParam == TextureParam::SwizzleB || CTParam == TextureParam::SwizzleA
			|| CTParam == TextureParam::WrapS || CTParam == TextureParam::WrapT
			|| CTParam == TextureParam::WrapR || CTParam == TextureParam::ImageFormatCompatibilityType
			, int> = 0>
			TextureParamValue getParameter() const
		{
			int p;
			glGetTextureParameteriv(*_id, utils::value(CTParam), &p);
			return static_cast<TextureParamValue>(p);
		}

		/**
		 * Gets value of float-valued texture parameter.
		 */
		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::LODBias
			|| CTParam == TextureParam::MinLOD || CTParam == TextureParam::MaxLOD
			, int> = 0>
			float getParameter() const
		{
			float v;
			glGetTextureParameterfv(*_id, utils::value(CTParam), &v);
			return v;
		}

		/**
		 * Sets integer-valued texture parameter.
		 */
		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::BaseLevel || CTParam == TextureParam::MaxLevel
			, int> = 0>
			void setParameter(int v) const { glTextureParameteri(*_id, utils::value(CTParam), v); }

		/**
		 * Sets float-valued texture parameter.
		 */
		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::LODBias
			|| CTParam == TextureParam::MinLOD || CTParam == TextureParam::MaxLOD
			, int> = 0>
			void setParameter(float v) const { glTextureParameterf(*_id, utils::value(CTParam), v); }

		/**
		 * Sets symbolic-valued texture parameter.
		 */
		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::DepthStencilMode
			|| CTParam == TextureParam::CompareFunc || CTParam == TextureParam::CompareMode
			|| CTParam == TextureParam::MinFilter || CTParam == TextureParam::MagFilter
			|| CTParam == TextureParam::SwizzleR || CTParam == TextureParam::SwizzleG
			|| CTParam == TextureParam::SwizzleB || CTParam == TextureParam::SwizzleA
			|| CTParam == TextureParam::WrapS || CTParam == TextureParam::WrapT
			|| CTParam == TextureParam::WrapR
			, int> = 0>
			void setParameter(TextureParamValue v) const { glTextureParameteri(*_id, utils::value(CTParam), utils::value(v)); }

		TextureBinding getBindingInfo() const { return TextureBinding(_target, _id); }
		TextureTarget getTarget() const { return _target; }
		TextureFormat getFormat() const { return _format; }

	protected:
		/**
		 * Registered target of the texture object.
		 */
		const TextureTarget _target;
		/**
		 * Registered internal format of the texture object.
		 */
		const TextureFormat _format;
		/**
		 * Whether storage requirements were set.
		 */
		bool _requirementsSet = false;
	};
}

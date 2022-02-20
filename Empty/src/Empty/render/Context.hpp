#pragma once

#include "glad/glad.h"

#include "Empty/render/gl/GLEnumsUtils.h"
#include "Empty/utils/EnumBitfield.h"
#include "Empty/utils/macros.h"
#include "Empty/utils/utils.hpp"

namespace Empty::render
{

	namespace gl
	{
		struct ProgramId;

		struct BufferInfo;
		struct FramebufferInfo;
		struct TextureInfo;
		struct TextureLevelInfo;
		struct VertexArrayInfo;

		struct ShaderProgram;
	}

	/**
	 * Structure used to carry general GL operations, not related to a specific GL object.
	 * `Context` has to be inherited by a structure that implements the `swap` method to
	 * be able to be instantiated, ideally also providing a window and GL context.
	 */
	struct Context
	{
		/**
		 * Typedef for GL debug message processor callbacks.
		 */
		using DebugCallback = void(*)(gl::DebugMessageSource, gl::DebugMessageType, gl::DebugMessageSeverity, int, const std::string&, const void*);

		const int frameWidth, frameHeight;
		// Color to clear color buffers with. Defaults to (0, 0, 0, 1).
		math::vec4 clearColor;
		// Depth value to clear depth buffers with. Defaults to 0.f.
		float clearDepth;
		// Stencil value to clear stencil buffers with. Defaults to 0.
		unsigned int clearStencil;

		Context(int width, int height, int major = 4, int minor = 5) : frameWidth(width), frameHeight(height), clearColor(0, 0, 0, 1), clearDepth(0.f), clearStencil(0) { }
		virtual ~Context() = default;

		/**
		 * Sets a callback function to be called with incoming debug messages, with some optional user data
		 * to be passed along with them.
		 */
		void debugMessageCallback(DebugCallback callback, const void* userData);

		/**
		 * Controls the reporting of debug messages by the context by setting up filters. For any filter,
		 * usage of the `DontCare` enum value selects every option of that filter. Although debug messages
		 * may be enabled in a non-debug context, it is not guaranteed that such a context will provide any
		 * amount of debug messages.
		 */
		void debugMessageControl(gl::DebugMessageSource source, gl::DebugMessageType type, gl::DebugMessageSeverity severity, bool enabled) const
		{
			glDebugMessageControl(utils::value(source), utils::value(type), utils::value(severity), 0, nullptr, enabled);
		}

		/**
		 * Swaps drawing buffers, essentially refreshing the screen.
		 */
		virtual void swap() const = 0;

		/**
		 * Bind a Buffer to the context.
		 */
		void bind(const gl::BufferInfo& b, gl::BufferTarget target) const;
		/**
		 * Bind a Buffer to an indexed target. Note this also binds the buffer on the
		 * generic binding point of the target, like the non-indexed `bind` method would.
		 */
		void bind(const gl::BufferInfo& b, gl::IndexedBufferTarget target, int index) const;
		/**
		 * Bind part of a Buffer to an indexed target. Note this also binds the buffer on
		 * the generic binding point of the target, like the non-indexed `bind` method would.
		 */
		void bind(const gl::BufferInfo& b, gl::IndexedBufferTarget target, int index, size_t offset, size_t size) const;
		/**
		 * Bind a Texture to the context.
		 */
		void bind(const gl::TextureInfo& t, int unit) const;
		/**
		 * Bind a specific level of a Texture to an image unit, with the given access policy and format for writes.
		 */
		void bind(const gl::TextureLevelInfo& t, int unit, gl::AccessPolicy access, gl::TextureFormat writeFormat) const;
		/**
		 * Bind a VertexArray to the context.
		 */
		void bind(const gl::VertexArrayInfo& va) const;

		/**
		 * Bind a Framebuffer to the context and set the viewport.
		 */
		void setFramebuffer(const gl::FramebufferInfo& fb, gl::FramebufferTarget target, int width, int height, int x = 0, int y = 0) const;

		/**
		 * Set the active ShaderProgram.
		 */
		void setShaderProgram(const gl::ShaderProgram& sp);

		/**
		 * Set the viewport dimensions.
		 */
		void setViewport(int width, int height, int x = 0, int y = 0) const { glViewport(x, y, width, height); }

		/**
		 * Reset the viewport dimensions to cover the entire default framebuffer.
		 */
		void resetViewport() const { glViewport(0, 0, frameWidth, frameHeight); }

		/**
		 * Draw a range of vertices from a bound VertexArray, arranged in successive primitives.
		 */
		void drawArrays(gl::PrimitiveType type, int first, int count) const { glDrawArrays(utils::value(type), first, count); }

		/**
		 * Draw a range of vertices from a bound VertexArray, arranged in primitives indexed by a
		 * the VertexArray's attached element buffer.
		 */
		void drawElements(gl::PrimitiveType type, gl::ElementType elemType, int first, int count) const
		{
			glDrawElements(utils::value(type), count, utils::value(elemType), reinterpret_cast<void*>(static_cast<size_t>(first)));
		}

		/**
		 * Launches a number of work groups to be processed by the compute shader stage of the currently active program.
		 */
		void dispatchCompute(int num_x, int num_y, int num_z) const
		{
			glDispatchCompute(num_x, num_y, num_z);
		}

		/**
		 * Clear a variety of drawing buffers with the corresponding colors.
		 */
		void clearBuffer(gl::DrawBufferType buffer) const;

		/**
		 * Sets up one or several memory barriers. The context will wait for operations of the given
		 * types to complete before submitting more commands.
		 */
		void memoryBarrier(utils::EnumBitfield<gl::MemoryBarrierType> barriers) const { glMemoryBarrier(barriers); }

		/**
		 * Enables a context capability.
		 */
		void enable(gl::ContextCapability cap) const
		{
			glEnable(utils::value(cap));
		}

		/**
		 * Disables a context capability.
		 */
		void disable(gl::ContextCapability cap) const
		{
			glDisable(utils::value(cap));
		}

		/**
		 * Enables an indexed context capability.
		 */
		template <gl::ContextCapability CTCap, std::enable_if_t<utils::isOneOf(CTCap, gl::ContextCapability::ClipDistance,
			gl::ContextCapability::Blend, gl::ContextCapability::ScissorTest),
			int> = 0>
		void enable(int index) const
		{
			if constexpr (CTCap == gl::ContextCapability::ClipDistance)
				glEnable(utils::value(CTCap) + index);
			else
				glEnablei(utils::value(CTCap), i);
		}

		/**
		 * Disables an indexed context capability.
		 */
		template <gl::ContextCapability CTCap, std::enable_if_t<utils::isOneOf(CTCap, gl::ContextCapability::ClipDistance,
			gl::ContextCapability::Blend, gl::ContextCapability::ScissorTest),
			int> = 0>
		void disable(int index) const
		{
			if constexpr (CTCap == gl::ContextCapability::ClipDistance)
				glDisable(utils::value(CTCap) + index);
			else
				glDisablei(utils::value(CTCap), i);
		}

#define DEF_STATE_VAR_ACCESSORS(type, Type, func) \
		template <gl::ContextStateVar CTVar, std::enable_if_t<gl::isStateVarNonIndexed(CTVar) && gl::isStateVar##Type##Valued(CTVar), int> = 0> \
		type getStateVar() const \
		{ \
			type v; \
			func##v(utils::value(CTVar), &v); \
			return v; \
		} \
		template <gl::ContextStateVar CTVar, std::enable_if_t<gl::isStateVarIndexed(CTVar) && gl::isStateVar##Type##Valued(CTVar), int> = 0> \
		type getStateVar(int index) const \
		{ \
			type v; \
			func##i_v(utils::value(CTVar), index, &v); \
			return v; \
		}

		DEF_STATE_VAR_ACCESSORS(int, Int, glGetInteger);
		DEF_STATE_VAR_ACCESSORS(float, Float, glGetFloat);
		DEF_STATE_VAR_ACCESSORS(math::vec2, Vec2, glGetFloat);
		DEF_STATE_VAR_ACCESSORS(math::vec4, Vec4, glGetFloat);
		DEF_STATE_VAR_ACCESSORS(math::ivec2, Ivec2, glGetInteger);
		DEF_STATE_VAR_ACCESSORS(math::ivec4, Ivec4, glGetInteger);
		DEF_STATE_VAR_ACCESSORS(bool, Bool, glGetBoolean);
#undef DEF_STATE_VAR_ACCESSORS

	private:
		/// State keeping
		
		// Currently bound shader program
		std::weak_ptr<gl::ProgramId> _currentProgram;

		// Information on current debug message callback
		struct DebugCallbackPayload
		{
			DebugCallback callback = nullptr;
			const void* userData = nullptr;
		} _debugCallbackPayload;
		static void _bypassDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
										 GLsizei, const GLchar* message, const void* userData)
		{
			auto* p = (DebugCallbackPayload*)userData;

			if(p->callback)
				p->callback(static_cast<render::gl::DebugMessageSource>(source), static_cast<render::gl::DebugMessageType>(type),
							static_cast<render::gl::DebugMessageSeverity>(severity), id, std::string(message), p->userData);
		}

	};
}

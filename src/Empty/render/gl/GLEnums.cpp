// This file is auto-generated. Do not edit !

#include "Empty/render/gl/GLEnums.hpp"

#include <unordered_map>

static const std::unordered_map<render::gl::PrimitiveType, const char*> PrimitiveType_names = {
	{render::gl::PrimitiveType::Points, "PrimitiveType::Points"},
	{render::gl::PrimitiveType::LineStrip, "PrimitiveType::LineStrip"},
	{render::gl::PrimitiveType::LineLoop, "PrimitiveType::LineLoop"},
	{render::gl::PrimitiveType::Lines, "PrimitiveType::Lines"},
	{render::gl::PrimitiveType::LineStripAdjacency, "PrimitiveType::LineStripAdjacency"},
	{render::gl::PrimitiveType::LinesAdjacency, "PrimitiveType::LinesAdjacency"},
	{render::gl::PrimitiveType::TriangleStrip, "PrimitiveType::TriangleStrip"},
	{render::gl::PrimitiveType::TriangleFan, "PrimitiveType::TriangleFan"},
	{render::gl::PrimitiveType::Triangles, "PrimitiveType::Triangles"},
	{render::gl::PrimitiveType::TriangleStripAdjacency, "PrimitiveType::TriangleStripAdjacency"},
	{render::gl::PrimitiveType::TrianglesAdjacency, "PrimitiveType::TrianglesAdjacency"},
	{render::gl::PrimitiveType::Patches, "PrimitiveType::Patches"},
};
template <> const char* utils::name<render::gl::PrimitiveType>(render::gl::PrimitiveType e) { return PrimitiveType_names.find(e)->second; }

static const std::unordered_map<render::gl::ElementType, const char*> ElementType_names = {
	{render::gl::ElementType::Byte, "ElementType::Byte"},
	{render::gl::ElementType::Short, "ElementType::Short"},
	{render::gl::ElementType::Int, "ElementType::Int"},
};
template <> const char* utils::name<render::gl::ElementType>(render::gl::ElementType e) { return ElementType_names.find(e)->second; }

static const std::unordered_map<render::gl::VertexAttribType, const char*> VertexAttribType_names = {
	{render::gl::VertexAttribType::UByte, "VertexAttribType::UByte"},
	{render::gl::VertexAttribType::Byte, "VertexAttribType::Byte"},
	{render::gl::VertexAttribType::UShort, "VertexAttribType::UShort"},
	{render::gl::VertexAttribType::Short, "VertexAttribType::Short"},
	{render::gl::VertexAttribType::UInt, "VertexAttribType::UInt"},
	{render::gl::VertexAttribType::Int, "VertexAttribType::Int"},
	{render::gl::VertexAttribType::Half, "VertexAttribType::Half"},
	{render::gl::VertexAttribType::Float, "VertexAttribType::Float"},
	{render::gl::VertexAttribType::Double, "VertexAttribType::Double"},
	{render::gl::VertexAttribType::Fixed, "VertexAttribType::Fixed"},
	{render::gl::VertexAttribType::UIntAAA2Rev, "VertexAttribType::UIntAAA2Rev"},
	{render::gl::VertexAttribType::IntAAA2Rev, "VertexAttribType::IntAAA2Rev"},
	{render::gl::VertexAttribType::UIntBfBfAfRev, "VertexAttribType::UIntBfBfAfRev"},
};
template <> const char* utils::name<render::gl::VertexAttribType>(render::gl::VertexAttribType e) { return VertexAttribType_names.find(e)->second; }

static const std::unordered_map<render::gl::BufferTarget, const char*> BufferTarget_names = {
	{render::gl::BufferTarget::Dynamic, "BufferTarget::Dynamic"},
	{render::gl::BufferTarget::Array, "BufferTarget::Array"},
	{render::gl::BufferTarget::AtomicCounter, "BufferTarget::AtomicCounter"},
	{render::gl::BufferTarget::CopyRead, "BufferTarget::CopyRead"},
	{render::gl::BufferTarget::CopyWrite, "BufferTarget::CopyWrite"},
	{render::gl::BufferTarget::DispatchIndirect, "BufferTarget::DispatchIndirect"},
	{render::gl::BufferTarget::DrawIndirect, "BufferTarget::DrawIndirect"},
	{render::gl::BufferTarget::ElementArray, "BufferTarget::ElementArray"},
	{render::gl::BufferTarget::PixelPack, "BufferTarget::PixelPack"},
	{render::gl::BufferTarget::PixelUnpack, "BufferTarget::PixelUnpack"},
	{render::gl::BufferTarget::Query, "BufferTarget::Query"},
	{render::gl::BufferTarget::ShaderStorage, "BufferTarget::ShaderStorage"},
	{render::gl::BufferTarget::Texture, "BufferTarget::Texture"},
	{render::gl::BufferTarget::TransformFeedback, "BufferTarget::TransformFeedback"},
	{render::gl::BufferTarget::Uniform, "BufferTarget::Uniform"},
};
template <> const char* utils::name<render::gl::BufferTarget>(render::gl::BufferTarget e) { return BufferTarget_names.find(e)->second; }

static const std::unordered_map<render::gl::BufferUsage, const char*> BufferUsage_names = {
	{render::gl::BufferUsage::StreamDraw, "BufferUsage::StreamDraw"},
	{render::gl::BufferUsage::StreamRead, "BufferUsage::StreamRead"},
	{render::gl::BufferUsage::StreamCopy, "BufferUsage::StreamCopy"},
	{render::gl::BufferUsage::StaticDraw, "BufferUsage::StaticDraw"},
	{render::gl::BufferUsage::StaticRead, "BufferUsage::StaticRead"},
	{render::gl::BufferUsage::StaticCopy, "BufferUsage::StaticCopy"},
	{render::gl::BufferUsage::DynamicDraw, "BufferUsage::DynamicDraw"},
	{render::gl::BufferUsage::DynamicRead, "BufferUsage::DynamicRead"},
	{render::gl::BufferUsage::DynamicCopy, "BufferUsage::DynamicCopy"},
};
template <> const char* utils::name<render::gl::BufferUsage>(render::gl::BufferUsage e) { return BufferUsage_names.find(e)->second; }

static const std::unordered_map<render::gl::BufferParam, const char*> BufferParam_names = {
	{render::gl::BufferParam::Access, "BufferParam::Access"},
	{render::gl::BufferParam::AccessFlags, "BufferParam::AccessFlags"},
	{render::gl::BufferParam::ImmutableStorage, "BufferParam::ImmutableStorage"},
	{render::gl::BufferParam::Mapped, "BufferParam::Mapped"},
	{render::gl::BufferParam::MapLength, "BufferParam::MapLength"},
	{render::gl::BufferParam::MapOffset, "BufferParam::MapOffset"},
	{render::gl::BufferParam::Size, "BufferParam::Size"},
	{render::gl::BufferParam::StorageFlags, "BufferParam::StorageFlags"},
	{render::gl::BufferParam::Usage, "BufferParam::Usage"},
};
template <> const char* utils::name<render::gl::BufferParam>(render::gl::BufferParam e) { return BufferParam_names.find(e)->second; }

static const std::unordered_map<render::gl::BufferAccess, const char*> BufferAccess_names = {
	{render::gl::BufferAccess::Invalid, "BufferAccess::Invalid"},
	{render::gl::BufferAccess::ReadOnly, "BufferAccess::ReadOnly"},
	{render::gl::BufferAccess::WriteOnly, "BufferAccess::WriteOnly"},
	{render::gl::BufferAccess::ReadWrite, "BufferAccess::ReadWrite"},
};
template <> const char* utils::name<render::gl::BufferAccess>(render::gl::BufferAccess e) { return BufferAccess_names.find(e)->second; }

static const std::unordered_map<render::gl::TextureTarget, const char*> TextureTarget_names = {
	{render::gl::TextureTarget::Dynamic, "TextureTarget::Dynamic"},
	{render::gl::TextureTarget::Texture1D, "TextureTarget::Texture1D"},
	{render::gl::TextureTarget::Texture2D, "TextureTarget::Texture2D"},
	{render::gl::TextureTarget::Texture3D, "TextureTarget::Texture3D"},
	{render::gl::TextureTarget::Texture1DArray, "TextureTarget::Texture1DArray"},
	{render::gl::TextureTarget::Texture2DArray, "TextureTarget::Texture2DArray"},
	{render::gl::TextureTarget::TextureRectangle, "TextureTarget::TextureRectangle"},
	{render::gl::TextureTarget::TextureCubeMap, "TextureTarget::TextureCubeMap"},
	{render::gl::TextureTarget::TextureCubeMapArray, "TextureTarget::TextureCubeMapArray"},
	{render::gl::TextureTarget::Texture2DMultisample, "TextureTarget::Texture2DMultisample"},
	{render::gl::TextureTarget::Texture2DMultisampleArray, "TextureTarget::Texture2DMultisampleArray"},
	{render::gl::TextureTarget::TextureBuffer, "TextureTarget::TextureBuffer"},
	{render::gl::TextureTarget::Proxy1D, "TextureTarget::Proxy1D"},
	{render::gl::TextureTarget::Proxy2D, "TextureTarget::Proxy2D"},
	{render::gl::TextureTarget::Proxy3D, "TextureTarget::Proxy3D"},
	{render::gl::TextureTarget::Proxy1DArray, "TextureTarget::Proxy1DArray"},
	{render::gl::TextureTarget::Proxy2DArray, "TextureTarget::Proxy2DArray"},
	{render::gl::TextureTarget::ProxyRectangle, "TextureTarget::ProxyRectangle"},
	{render::gl::TextureTarget::ProxyCubeMap, "TextureTarget::ProxyCubeMap"},
	{render::gl::TextureTarget::ProxyCubeMapArray, "TextureTarget::ProxyCubeMapArray"},
	{render::gl::TextureTarget::Proxy2DMultisample, "TextureTarget::Proxy2DMultisample"},
	{render::gl::TextureTarget::Proxy2DMultisampleArray, "TextureTarget::Proxy2DMultisampleArray"},
};
template <> const char* utils::name<render::gl::TextureTarget>(render::gl::TextureTarget e) { return TextureTarget_names.find(e)->second; }

static const std::unordered_map<render::gl::TextureFormat, const char*> TextureFormat_names = {
	{render::gl::TextureFormat::Dynamic, "TextureFormat::Dynamic"},
	{render::gl::TextureFormat::Red, "TextureFormat::Red"},
	{render::gl::TextureFormat::RG, "TextureFormat::RG"},
	{render::gl::TextureFormat::RGB, "TextureFormat::RGB"},
	{render::gl::TextureFormat::RGBA, "TextureFormat::RGBA"},
	{render::gl::TextureFormat::Depth, "TextureFormat::Depth"},
	{render::gl::TextureFormat::DepthStencil, "TextureFormat::DepthStencil"},
	{render::gl::TextureFormat::Red8, "TextureFormat::Red8"},
	{render::gl::TextureFormat::Red8s, "TextureFormat::Red8s"},
	{render::gl::TextureFormat::Red16, "TextureFormat::Red16"},
	{render::gl::TextureFormat::Red16s, "TextureFormat::Red16s"},
	{render::gl::TextureFormat::RG8, "TextureFormat::RG8"},
	{render::gl::TextureFormat::RG8s, "TextureFormat::RG8s"},
	{render::gl::TextureFormat::RG16, "TextureFormat::RG16"},
	{render::gl::TextureFormat::RG16s, "TextureFormat::RG16s"},
	{render::gl::TextureFormat::RGB332, "TextureFormat::RGB332"},
	{render::gl::TextureFormat::RGB4, "TextureFormat::RGB4"},
	{render::gl::TextureFormat::RGB5, "TextureFormat::RGB5"},
	{render::gl::TextureFormat::RGB8, "TextureFormat::RGB8"},
	{render::gl::TextureFormat::RGB8s, "TextureFormat::RGB8s"},
	{render::gl::TextureFormat::RGB10, "TextureFormat::RGB10"},
	{render::gl::TextureFormat::RGB12, "TextureFormat::RGB12"},
	{render::gl::TextureFormat::RGB16s, "TextureFormat::RGB16s"},
	{render::gl::TextureFormat::RGBA2, "TextureFormat::RGBA2"},
	{render::gl::TextureFormat::RGBA4, "TextureFormat::RGBA4"},
	{render::gl::TextureFormat::RGB5A1, "TextureFormat::RGB5A1"},
	{render::gl::TextureFormat::RGBA8, "TextureFormat::RGBA8"},
	{render::gl::TextureFormat::RGBA8s, "TextureFormat::RGBA8s"},
	{render::gl::TextureFormat::RGB10A2, "TextureFormat::RGB10A2"},
	{render::gl::TextureFormat::RGB10A2ui, "TextureFormat::RGB10A2ui"},
	{render::gl::TextureFormat::RGBA12, "TextureFormat::RGBA12"},
	{render::gl::TextureFormat::RGBA16, "TextureFormat::RGBA16"},
	{render::gl::TextureFormat::SRGB8, "TextureFormat::SRGB8"},
	{render::gl::TextureFormat::SRGBA8, "TextureFormat::SRGBA8"},
	{render::gl::TextureFormat::Red16f, "TextureFormat::Red16f"},
	{render::gl::TextureFormat::RG16f, "TextureFormat::RG16f"},
	{render::gl::TextureFormat::RGB16f, "TextureFormat::RGB16f"},
	{render::gl::TextureFormat::RGBA16f, "TextureFormat::RGBA16f"},
	{render::gl::TextureFormat::Red32f, "TextureFormat::Red32f"},
	{render::gl::TextureFormat::RG32f, "TextureFormat::RG32f"},
	{render::gl::TextureFormat::RGB32f, "TextureFormat::RGB32f"},
	{render::gl::TextureFormat::RGBA32f, "TextureFormat::RGBA32f"},
	{render::gl::TextureFormat::R11G11B10f, "TextureFormat::R11G11B10f"},
	{render::gl::TextureFormat::RGB9E5, "TextureFormat::RGB9E5"},
	{render::gl::TextureFormat::Red8i, "TextureFormat::Red8i"},
	{render::gl::TextureFormat::Red8ui, "TextureFormat::Red8ui"},
	{render::gl::TextureFormat::Red16i, "TextureFormat::Red16i"},
	{render::gl::TextureFormat::Red16ui, "TextureFormat::Red16ui"},
	{render::gl::TextureFormat::Red32i, "TextureFormat::Red32i"},
	{render::gl::TextureFormat::Red32ui, "TextureFormat::Red32ui"},
	{render::gl::TextureFormat::RG8i, "TextureFormat::RG8i"},
	{render::gl::TextureFormat::RG8ui, "TextureFormat::RG8ui"},
	{render::gl::TextureFormat::RG16i, "TextureFormat::RG16i"},
	{render::gl::TextureFormat::RG16ui, "TextureFormat::RG16ui"},
	{render::gl::TextureFormat::RG32i, "TextureFormat::RG32i"},
	{render::gl::TextureFormat::RG32ui, "TextureFormat::RG32ui"},
	{render::gl::TextureFormat::RGB8i, "TextureFormat::RGB8i"},
	{render::gl::TextureFormat::RGB8ui, "TextureFormat::RGB8ui"},
	{render::gl::TextureFormat::RGB16i, "TextureFormat::RGB16i"},
	{render::gl::TextureFormat::RGB16ui, "TextureFormat::RGB16ui"},
	{render::gl::TextureFormat::RGB32i, "TextureFormat::RGB32i"},
	{render::gl::TextureFormat::RGB32ui, "TextureFormat::RGB32ui"},
	{render::gl::TextureFormat::RGBA8i, "TextureFormat::RGBA8i"},
	{render::gl::TextureFormat::RGBA8ui, "TextureFormat::RGBA8ui"},
	{render::gl::TextureFormat::RGBA16i, "TextureFormat::RGBA16i"},
	{render::gl::TextureFormat::RGBA16ui, "TextureFormat::RGBA16ui"},
	{render::gl::TextureFormat::RGBA32i, "TextureFormat::RGBA32i"},
	{render::gl::TextureFormat::RGBA32ui, "TextureFormat::RGBA32ui"},
	{render::gl::TextureFormat::RedComp, "TextureFormat::RedComp"},
	{render::gl::TextureFormat::RGComp, "TextureFormat::RGComp"},
	{render::gl::TextureFormat::RGBComp, "TextureFormat::RGBComp"},
	{render::gl::TextureFormat::RGBAComp, "TextureFormat::RGBAComp"},
	{render::gl::TextureFormat::SRGBComp, "TextureFormat::SRGBComp"},
	{render::gl::TextureFormat::SRGBAComp, "TextureFormat::SRGBAComp"},
};
template <> const char* utils::name<render::gl::TextureFormat>(render::gl::TextureFormat e) { return TextureFormat_names.find(e)->second; }

static const std::unordered_map<render::gl::CubeMapFace, const char*> CubeMapFace_names = {
	{render::gl::CubeMapFace::PlusX, "CubeMapFace::PlusX"},
	{render::gl::CubeMapFace::MinusX, "CubeMapFace::MinusX"},
	{render::gl::CubeMapFace::PlusY, "CubeMapFace::PlusY"},
	{render::gl::CubeMapFace::MinusY, "CubeMapFace::MinusY"},
	{render::gl::CubeMapFace::PlusZ, "CubeMapFace::PlusZ"},
	{render::gl::CubeMapFace::MinusZ, "CubeMapFace::MinusZ"},
};
template <> const char* utils::name<render::gl::CubeMapFace>(render::gl::CubeMapFace e) { return CubeMapFace_names.find(e)->second; }

static const std::unordered_map<render::gl::PixelFormat, const char*> PixelFormat_names = {
	{render::gl::PixelFormat::Red, "PixelFormat::Red"},
	{render::gl::PixelFormat::RG, "PixelFormat::RG"},
	{render::gl::PixelFormat::RGB, "PixelFormat::RGB"},
	{render::gl::PixelFormat::BGR, "PixelFormat::BGR"},
	{render::gl::PixelFormat::RGBA, "PixelFormat::RGBA"},
	{render::gl::PixelFormat::BGRA, "PixelFormat::BGRA"},
	{render::gl::PixelFormat::RedInt, "PixelFormat::RedInt"},
	{render::gl::PixelFormat::RGInt, "PixelFormat::RGInt"},
	{render::gl::PixelFormat::RGBInt, "PixelFormat::RGBInt"},
	{render::gl::PixelFormat::BGRInt, "PixelFormat::BGRInt"},
	{render::gl::PixelFormat::RGBAInt, "PixelFormat::RGBAInt"},
	{render::gl::PixelFormat::BGRAInt, "PixelFormat::BGRAInt"},
	{render::gl::PixelFormat::Depth, "PixelFormat::Depth"},
	{render::gl::PixelFormat::Stencil, "PixelFormat::Stencil"},
	{render::gl::PixelFormat::DepthStencil, "PixelFormat::DepthStencil"},
};
template <> const char* utils::name<render::gl::PixelFormat>(render::gl::PixelFormat e) { return PixelFormat_names.find(e)->second; }

static const std::unordered_map<render::gl::PixelType, const char*> PixelType_names = {
	{render::gl::PixelType::UByte, "PixelType::UByte"},
	{render::gl::PixelType::Byte, "PixelType::Byte"},
	{render::gl::PixelType::UShort, "PixelType::UShort"},
	{render::gl::PixelType::Short, "PixelType::Short"},
	{render::gl::PixelType::UInt, "PixelType::UInt"},
	{render::gl::PixelType::Int, "PixelType::Int"},
	{render::gl::PixelType::Half, "PixelType::Half"},
	{render::gl::PixelType::Float, "PixelType::Float"},
	{render::gl::PixelType::UByte332, "PixelType::UByte332"},
	{render::gl::PixelType::UByte332Rev, "PixelType::UByte332Rev"},
	{render::gl::PixelType::UShort565, "PixelType::UShort565"},
	{render::gl::PixelType::UShort565Rev, "PixelType::UShort565Rev"},
	{render::gl::PixelType::UShort4, "PixelType::UShort4"},
	{render::gl::PixelType::UShort4Rev, "PixelType::UShort4Rev"},
	{render::gl::PixelType::UShort5551, "PixelType::UShort5551"},
	{render::gl::PixelType::UShort5551Rev, "PixelType::UShort5551Rev"},
	{render::gl::PixelType::UInt8, "PixelType::UInt8"},
	{render::gl::PixelType::UInt8Rev, "PixelType::UInt8Rev"},
	{render::gl::PixelType::UIntAAA2, "PixelType::UIntAAA2"},
	{render::gl::PixelType::UIntAAA2Rev, "PixelType::UIntAAA2Rev"},
};
template <> const char* utils::name<render::gl::PixelType>(render::gl::PixelType e) { return PixelType_names.find(e)->second; }

static const std::unordered_map<render::gl::TextureParam, const char*> TextureParam_names = {
	{render::gl::TextureParam::DepthStencilMode, "TextureParam::DepthStencilMode"},
	{render::gl::TextureParam::BaseLevel, "TextureParam::BaseLevel"},
	{render::gl::TextureParam::BorderColor, "TextureParam::BorderColor"},
	{render::gl::TextureParam::CompareFunc, "TextureParam::CompareFunc"},
	{render::gl::TextureParam::CompareMode, "TextureParam::CompareMode"},
	{render::gl::TextureParam::LODBias, "TextureParam::LODBias"},
	{render::gl::TextureParam::MinFilter, "TextureParam::MinFilter"},
	{render::gl::TextureParam::MagFilter, "TextureParam::MagFilter"},
	{render::gl::TextureParam::MinLOD, "TextureParam::MinLOD"},
	{render::gl::TextureParam::MaxLOD, "TextureParam::MaxLOD"},
	{render::gl::TextureParam::MaxLevel, "TextureParam::MaxLevel"},
	{render::gl::TextureParam::SwizzleR, "TextureParam::SwizzleR"},
	{render::gl::TextureParam::SwizzleG, "TextureParam::SwizzleG"},
	{render::gl::TextureParam::SwizzleB, "TextureParam::SwizzleB"},
	{render::gl::TextureParam::SwizzleA, "TextureParam::SwizzleA"},
	{render::gl::TextureParam::SwizzleRGBA, "TextureParam::SwizzleRGBA"},
	{render::gl::TextureParam::WrapS, "TextureParam::WrapS"},
	{render::gl::TextureParam::WrapT, "TextureParam::WrapT"},
	{render::gl::TextureParam::WrapR, "TextureParam::WrapR"},
	{render::gl::TextureParam::ViewImmutableLevels, "TextureParam::ViewImmutableLevels"},
	{render::gl::TextureParam::ViewMinLayer, "TextureParam::ViewMinLayer"},
	{render::gl::TextureParam::ViewNumLayers, "TextureParam::ViewNumLayers"},
	{render::gl::TextureParam::ViewMinLevel, "TextureParam::ViewMinLevel"},
	{render::gl::TextureParam::ViewNumLevels, "TextureParam::ViewNumLevels"},
	{render::gl::TextureParam::ImageFormatCompatibilityType, "TextureParam::ImageFormatCompatibilityType"},
	{render::gl::TextureParam::HasImmutableFormat, "TextureParam::HasImmutableFormat"},
};
template <> const char* utils::name<render::gl::TextureParam>(render::gl::TextureParam e) { return TextureParam_names.find(e)->second; }

static const std::unordered_map<render::gl::TextureParamValue, const char*> TextureParamValue_names = {
	{render::gl::TextureParamValue::Invalid, "TextureParamValue::Invalid"},
	{render::gl::TextureParamValue::DepthMode, "TextureParamValue::DepthMode"},
	{render::gl::TextureParamValue::StencilMode, "TextureParamValue::StencilMode"},
	{render::gl::TextureParamValue::FuncLEqual, "TextureParamValue::FuncLEqual"},
	{render::gl::TextureParamValue::FuncGEqual, "TextureParamValue::FuncGEqual"},
	{render::gl::TextureParamValue::FuncLess, "TextureParamValue::FuncLess"},
	{render::gl::TextureParamValue::FuncGreater, "TextureParamValue::FuncGreater"},
	{render::gl::TextureParamValue::FuncEqual, "TextureParamValue::FuncEqual"},
	{render::gl::TextureParamValue::FuncNotEqual, "TextureParamValue::FuncNotEqual"},
	{render::gl::TextureParamValue::FuncAlways, "TextureParamValue::FuncAlways"},
	{render::gl::TextureParamValue::FuncNever, "TextureParamValue::FuncNever"},
	{render::gl::TextureParamValue::CompareRefToTexture, "TextureParamValue::CompareRefToTexture"},
	{render::gl::TextureParamValue::NoCompare, "TextureParamValue::NoCompare"},
	{render::gl::TextureParamValue::FilterNearest, "TextureParamValue::FilterNearest"},
	{render::gl::TextureParamValue::FilterLinear, "TextureParamValue::FilterLinear"},
	{render::gl::TextureParamValue::FilterNearestMipmapNearest, "TextureParamValue::FilterNearestMipmapNearest"},
	{render::gl::TextureParamValue::FilterNearestMipmapLinear, "TextureParamValue::FilterNearestMipmapLinear"},
	{render::gl::TextureParamValue::FilterLinearMipmapNearest, "TextureParamValue::FilterLinearMipmapNearest"},
	{render::gl::TextureParamValue::FilterLinearMipmapLinear, "TextureParamValue::FilterLinearMipmapLinear"},
	{render::gl::TextureParamValue::SwizzleRed, "TextureParamValue::SwizzleRed"},
	{render::gl::TextureParamValue::SwizzleGreen, "TextureParamValue::SwizzleGreen"},
	{render::gl::TextureParamValue::SwizzleBlue, "TextureParamValue::SwizzleBlue"},
	{render::gl::TextureParamValue::SwizzleAlpha, "TextureParamValue::SwizzleAlpha"},
	{render::gl::TextureParamValue::SwizzleZero, "TextureParamValue::SwizzleZero"},
	{render::gl::TextureParamValue::SwizzleOne, "TextureParamValue::SwizzleOne"},
	{render::gl::TextureParamValue::ClampToEdge, "TextureParamValue::ClampToEdge"},
	{render::gl::TextureParamValue::ClampToBorder, "TextureParamValue::ClampToBorder"},
	{render::gl::TextureParamValue::MirrorRepeat, "TextureParamValue::MirrorRepeat"},
	{render::gl::TextureParamValue::Repeat, "TextureParamValue::Repeat"},
	{render::gl::TextureParamValue::MirrorClampToEdge, "TextureParamValue::MirrorClampToEdge"},
};
template <> const char* utils::name<render::gl::TextureParamValue>(render::gl::TextureParamValue e) { return TextureParamValue_names.find(e)->second; }

static const std::unordered_map<render::gl::ShaderType, const char*> ShaderType_names = {
	{render::gl::ShaderType::Compute, "ShaderType::Compute"},
	{render::gl::ShaderType::Fragment, "ShaderType::Fragment"},
	{render::gl::ShaderType::Geometry, "ShaderType::Geometry"},
	{render::gl::ShaderType::TessControl, "ShaderType::TessControl"},
	{render::gl::ShaderType::TessEvaluation, "ShaderType::TessEvaluation"},
	{render::gl::ShaderType::Vertex, "ShaderType::Vertex"},
};
template <> const char* utils::name<render::gl::ShaderType>(render::gl::ShaderType e) { return ShaderType_names.find(e)->second; }

static const std::unordered_map<render::gl::ShaderParam, const char*> ShaderParam_names = {
	{render::gl::ShaderParam::Type, "ShaderParam::Type"},
	{render::gl::ShaderParam::DeleteStatus, "ShaderParam::DeleteStatus"},
	{render::gl::ShaderParam::CompileStatus, "ShaderParam::CompileStatus"},
	{render::gl::ShaderParam::InfoLogLength, "ShaderParam::InfoLogLength"},
	{render::gl::ShaderParam::SourceLength, "ShaderParam::SourceLength"},
};
template <> const char* utils::name<render::gl::ShaderParam>(render::gl::ShaderParam e) { return ShaderParam_names.find(e)->second; }

static const std::unordered_map<render::gl::ProgramParam, const char*> ProgramParam_names = {
	{render::gl::ProgramParam::DeleteStatus, "ProgramParam::DeleteStatus"},
	{render::gl::ProgramParam::LinkStatus, "ProgramParam::LinkStatus"},
	{render::gl::ProgramParam::ValidateStatus, "ProgramParam::ValidateStatus"},
	{render::gl::ProgramParam::InfoLogLength, "ProgramParam::InfoLogLength"},
	{render::gl::ProgramParam::AttachedShaders, "ProgramParam::AttachedShaders"},
	{render::gl::ProgramParam::ActiveAtomicCounterBuffers, "ProgramParam::ActiveAtomicCounterBuffers"},
	{render::gl::ProgramParam::ActiveAttributes, "ProgramParam::ActiveAttributes"},
	{render::gl::ProgramParam::ActiveAttributeMaxNameLength, "ProgramParam::ActiveAttributeMaxNameLength"},
	{render::gl::ProgramParam::ActiveUniforms, "ProgramParam::ActiveUniforms"},
	{render::gl::ProgramParam::ActiveUniformMaxNameLength, "ProgramParam::ActiveUniformMaxNameLength"},
	{render::gl::ProgramParam::BinaryLength, "ProgramParam::BinaryLength"},
	{render::gl::ProgramParam::ComputeWorkGroupSize, "ProgramParam::ComputeWorkGroupSize"},
	{render::gl::ProgramParam::TransformFeedbackBufferMode, "ProgramParam::TransformFeedbackBufferMode"},
	{render::gl::ProgramParam::TransformFeedbackVaryings, "ProgramParam::TransformFeedbackVaryings"},
	{render::gl::ProgramParam::TransformFeedbackVaryingMaxNameLength, "ProgramParam::TransformFeedbackVaryingMaxNameLength"},
	{render::gl::ProgramParam::GeometryVerticesOut, "ProgramParam::GeometryVerticesOut"},
	{render::gl::ProgramParam::GeometryInputType, "ProgramParam::GeometryInputType"},
	{render::gl::ProgramParam::GeometryOutputType, "ProgramParam::GeometryOutputType"},
	{render::gl::ProgramParam::RetrievableBinaryHint, "ProgramParam::RetrievableBinaryHint"},
	{render::gl::ProgramParam::Separable, "ProgramParam::Separable"},
};
template <> const char* utils::name<render::gl::ProgramParam>(render::gl::ProgramParam e) { return ProgramParam_names.find(e)->second; }


# Empty
Convenience wrapper classes for OpenGL things, geared towards type safety.

## Requirements

Right now the library is designed to be included as a git submodule. It should work out of the box with CMake (and Python 3 for generating a couple files). I personally use Visual Studio 2019 to work on it, so I know it works on that.

## tl;dr

- Wraps OpenGL constants in scoped enums for maximum type safety
- Wraps OpenGL resources (VAOs, buffers, textures etc) in convenient classes for localized functionality
- Provides top-level context class for general-purpose things and stuffs
- Replicates GLSL on the CPU as much as it can for easy CPU debugging of your shaders
- Does not overstep its boundaries and sticks to making using OpenGL easier and safer. Most functions are thus inlined one-liners with 0 runtime overhead compared to just using OpenGL.

## ~~The library is not ready for use yet. Feel free to browse around the code to see how things are done, but that's about all you can do right now.~~
## The library is usable but does not do much. We are currently working on an example package and adding things to it as usage demands them. You should do the same ! :D

The idea for this library was initiated by me working with OpenGL and asking myself the following questions :
- What format did I give that texture again ?
- What GLenums does that parameter accept ?
- Is this a buffer or texture ID ..?
- and many more like that.

Empty leverages C++'s scoped enums, templating and [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae) to wrap OpenGL resources and functions with type information for air-tight compile-time checks and type safety. It doesn't actually offer any runtime functionality outside of this, hence the name, "Empty".

## Comparison

Here is how you would use Empty in place of traditional OpenGL code.

<table>
<tr>
<th>Traditional OpenGL</th><th>With Empty</th>
</tr>
<tr>
<td>

```cpp
GLuint tex;
glGenTextures(1, &tex);
glBindTexture(GL_TEXTURE_2D, tex);
glTexParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
int param;
glGetTexParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &param);
std::cout << param << std::endl; // displays '9729'
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 64, 64, 0, GL_RGBA, GL_BYTE, nullptr);
glBindTexture(GL_TEXTURE_2D, 0);
```

</td>
<td>

```cpp
Texture<TextureTarget::Texture2D, TextureFormat::RGBA8> tex;
tex.bind();
tex.setParameter<TextureParam::MinFilter>(TextureParamValue::FilterLinear);
std::cout << utils::name(tex.getParameter<TextureParam::MagFilter>()) << std::endl; // displays 'TextureParamValue::FilterLinear'
tex.uploadData(0, 64, 64, PixelFormat::RGBA, PixelType::Byte, nullptr);
tex.unbind();
```

</td>
</tr>
</table>

As you can see, symbolic constants are no longer meaningless integers coming from obscure `#define` lines, and are instead wrapped in scoped enums that provide compile-time type safety.

Another feature that is less obvious in this snippet is that Empty leverages SFINAE to only present relevant overloads of methods.
For example, say you want to set a bunch of texture parameters. Note how you have to write the parameter's name as a template parameter rather than a method argument ; this makes it so you are presented with the correct overload of the `setParameter` method, depending on the type of value that this specific parameter accepts.
Likewise, the correct overload of the `Texture::uploadData` method is chosen based on the texture's dimensionality (deduced from the target template parameter), and will use the correct function among `glTexImage1D`, `glTexImage2D` or `glTexImage3D`.

Special `Dynamic` default enum values are also available in case a class's parameters are not known at compile time. This makes it so you can for example instantiate a texture with runtime-provided targets and formats. Those dynamic enum values are taken into account in SFINAE statements and the correct method overloads will be provided accordingly.

```cpp
Texture<> makeTexture(TextureTarget target, TextureFormat format)
{
    // return Texture<TextureTarget::Dynamic, TextureFormat::RGBA8>(target, format); // compilation error
    return Texture<>(target, format); // the 2-argument constructor is only available with dynamic target and format
}
```

A similar treatment is planned for just about every OpenGL resource.

Additional features include the utility functions `utils::value` and `utils::name`. The former casts a scoped enum value into its underlying type and is used to communicate with OpenGL functions (for example, `utils::value(TextureParamValue::FilterLinear)` returns `GL_LINEAR`) ; the latter returns a string representation of a scoped enum value's name (`utils::name(TextureTarget::Texture2D)` returns a `const char*` reading `"TextureTarget::Texture2D"`).

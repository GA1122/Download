void TextureMapperGL::endClip()
{
    data().globalGLData.stencilIndex >>= 1;
    glStencilFunc(data().globalGLData.stencilIndex > 1 ? GL_EQUAL : GL_ALWAYS, data().globalGLData.stencilIndex - 1, data().globalGLData.stencilIndex - 1);
}

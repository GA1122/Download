void UpdateBoundTexturePassthroughSize(gl::GLApi* api,
                                       TexturePassthrough* texture) {
  GLint texture_memory_size = 0;
  api->glGetTexParameterivFn(texture->target(), GL_MEMORY_SIZE_ANGLE,
                             &texture_memory_size);

  texture->SetEstimatedSize(texture_memory_size);
}

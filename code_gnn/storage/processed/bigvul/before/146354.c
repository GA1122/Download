void WebGLRenderingContextBase::Reshape(int width, int height) {
  if (isContextLost())
    return;

  GLint buffer = 0;
  if (IsWebGL2OrHigher()) {
    ContextGL()->GetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &buffer);
    if (buffer) {
      ContextGL()->BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }
  }

  GLint max_size = std::min(max_texture_size_, max_renderbuffer_size_);
  GLint max_width = std::min(max_size, max_viewport_dims_[0]);
  GLint max_height = std::min(max_size, max_viewport_dims_[1]);
  width = Clamp(width, 1, max_width);
  height = Clamp(height, 1, max_height);

  const int kMaxArea = 4096 * 4096;
  int current_area = width * height;
  if (current_area > kMaxArea) {
    float scale_factor =
        sqrtf(static_cast<float>(kMaxArea) / static_cast<float>(current_area));
    width = std::max(1, static_cast<int>(width * scale_factor));
    height = std::max(1, static_cast<int>(height * scale_factor));
  }

  GetDrawingBuffer()->Resize(IntSize(width, height));

  if (buffer) {
    ContextGL()->BindBuffer(GL_PIXEL_UNPACK_BUFFER,
                            static_cast<GLuint>(buffer));
  }
}

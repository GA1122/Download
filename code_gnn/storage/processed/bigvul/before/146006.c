IntRect WebGL2RenderingContextBase::GetTextureSourceSubRectangle(
    GLsizei width,
    GLsizei height) {
  return IntRect(unpack_skip_pixels_, unpack_skip_rows_, width, height);
}

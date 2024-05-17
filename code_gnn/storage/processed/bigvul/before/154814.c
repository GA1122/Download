error::Error GLES2DecoderPassthroughImpl::DoSwapBuffersWithBoundsCHROMIUM(
    uint64_t swap_id,
    GLsizei count,
    const volatile GLint* rects,
    GLbitfield flags) {
  if (count < 0) {
    InsertError(GL_INVALID_VALUE, "count cannot be negative.");
    return error::kNoError;
  }

  std::vector<gfx::Rect> bounds(count);
  for (GLsizei i = 0; i < count; ++i) {
    bounds[i] = gfx::Rect(rects[i * 4 + 0], rects[i * 4 + 1], rects[i * 4 + 2],
                          rects[i * 4 + 3]);
  }

  client()->OnSwapBuffers(swap_id, flags);
  return CheckSwapBuffersResult(
      surface_->SwapBuffersWithBounds(bounds, base::DoNothing()),
      "SwapBuffersWithBounds");
}

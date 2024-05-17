void GLES2Implementation::PartialSwapBuffers(
    const gfx::Rect& sub_buffer,
    uint32_t flags,
    SwapCompletedCallback swap_completed,
    PresentationCallback presentation_callback) {
  PostSubBufferCHROMIUM(PrepareNextSwapId(std::move(swap_completed),
                                          std::move(presentation_callback)),
                        sub_buffer.x(), sub_buffer.y(), sub_buffer.width(),
                        sub_buffer.height(), flags);
}

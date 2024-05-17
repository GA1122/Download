void GLES2Implementation::Swap(uint32_t flags,
                               SwapCompletedCallback complete_callback,
                               PresentationCallback presentation_callback) {
  SwapBuffers(PrepareNextSwapId(std::move(complete_callback),
                                std::move(presentation_callback)),
              flags);
}

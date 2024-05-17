void GLES2Implementation::CommitOverlayPlanes(
    uint32_t flags,
    SwapCompletedCallback swap_completed,
    PresentationCallback presentation_callback) {
  CommitOverlayPlanesCHROMIUM(
      PrepareNextSwapId(std::move(swap_completed),
                        std::move(presentation_callback)),
      flags);
}

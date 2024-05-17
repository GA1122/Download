error::Error GLES2DecoderPassthroughImpl::DoCommitOverlayPlanesCHROMIUM(
    uint64_t swap_id,
    GLbitfield flags) {
  if (!surface_->SupportsCommitOverlayPlanes()) {
    InsertError(GL_INVALID_OPERATION,
                "glCommitOverlayPlanes not supported by surface.");
    return error::kNoError;
  }

  client()->OnSwapBuffers(swap_id, flags);
  return CheckSwapBuffersResult(
      surface_->CommitOverlayPlanes(base::DoNothing()), "CommitOverlayPlanes");
}

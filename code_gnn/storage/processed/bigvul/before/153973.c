void GLES2DecoderImpl::DoCommitOverlayPlanes(uint64_t swap_id,
                                             GLbitfield flags) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoCommitOverlayPlanes");
  if (!supports_commit_overlay_planes_) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glCommitOverlayPlanes",
                       "command not supported by surface");
    return;
  }
  ClearScheduleCALayerState();
  if (supports_async_swap_) {
    client()->OnSwapBuffers(swap_id, flags);
    surface_->CommitOverlayPlanesAsync(
        base::BindOnce(&GLES2DecoderImpl::FinishAsyncSwapBuffers,
                       weak_ptr_factory_.GetWeakPtr(), swap_id),
        base::DoNothing());
  } else {
    client()->OnSwapBuffers(swap_id, flags);
    FinishSwapBuffers(surface_->CommitOverlayPlanes(base::DoNothing()));
  }
}

SkiaOutputSurfaceImpl::ScopedPaint::ScopedPaint(
    SkSurfaceCharacterization characterization,
    RenderPassId render_pass_id)
    : render_pass_id_(render_pass_id) {
  recorder_storage_.emplace(characterization);
  recorder_ = &recorder_storage_.value();
}

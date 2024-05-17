void SkiaOutputSurfaceImpl::RecreateRootRecorder() {
  DCHECK(characterization_.isValid());
  root_recorder_.emplace(characterization_);

  ignore_result(root_recorder_->getCanvas());
}

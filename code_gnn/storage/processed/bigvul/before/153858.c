GLES2Implementation::DeferErrorCallbacks::~DeferErrorCallbacks() {
  DCHECK_EQ(true, gles2_implementation_->deferring_error_callbacks_);
  gles2_implementation_->deferring_error_callbacks_ = false;
  gles2_implementation_->CallDeferredErrorCallbacks();
}

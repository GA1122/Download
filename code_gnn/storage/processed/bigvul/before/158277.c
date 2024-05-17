TouchEmulator* RenderWidgetHostImpl::GetExistingTouchEmulator() {
  if (!delegate_ || !delegate_->GetInputEventRouter() ||
      !delegate_->GetInputEventRouter()->has_touch_emulator()) {
    return nullptr;
  }

  return delegate_->GetInputEventRouter()->GetTouchEmulator();
}

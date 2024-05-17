void ClientControlledShellSurface::OnSetFrame(SurfaceFrameType type) {
  if (container_ == ash::kShellWindowId_SystemModalContainer &&
      type != SurfaceFrameType::NONE) {
    LOG(WARNING)
        << "A surface in system modal container should not have a frame:"
        << static_cast<int>(type);
    return;
  }

  EventTargetingBlocker blocker;
  bool suppress_mouse_event = frame_type_ != type && widget_;
  if (suppress_mouse_event)
    blocker.Block(widget_->GetNativeWindow());
  ShellSurfaceBase::OnSetFrame(type);
  UpdateAutoHideFrame();

  if (suppress_mouse_event)
    UpdateSurfaceBounds();
}

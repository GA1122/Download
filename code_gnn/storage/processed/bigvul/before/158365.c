void RenderWidgetHostImpl::SetMouseCapture(bool capture) {
  if (delegate_)
    delegate_->GetInputEventRouter()->SetMouseCaptureTarget(GetView(), capture);
}

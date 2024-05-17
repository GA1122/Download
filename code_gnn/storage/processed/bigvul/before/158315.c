void RenderWidgetHostImpl::OnInputEventAckTimeout() {
  RendererIsUnresponsive(base::BindRepeating(
      &RenderWidgetHostImpl::RestartInputEventAckTimeoutIfNecessary,
      weak_factory_.GetWeakPtr()));
}

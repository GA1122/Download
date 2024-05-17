void RenderWidgetHostViewAura::DidStopFlinging() {
  selection_controller_client_->OnScrollCompleted();
}

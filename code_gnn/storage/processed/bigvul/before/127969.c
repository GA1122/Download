void BrowserViewRenderer::UpdateParentDrawConstraints() {
  PostInvalidateWithFallback();
  ParentCompositorDrawConstraints parent_draw_constraints =
      shared_renderer_state_.GetParentDrawConstraintsOnUI();
  client_->ParentDrawConstraintsUpdated(parent_draw_constraints);
}

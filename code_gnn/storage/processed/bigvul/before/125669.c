void RenderViewHostImpl::OnRenderViewGone(int status, int exit_code) {
  render_view_termination_status_ =
      static_cast<base::TerminationStatus>(status);

  ClearPowerSaveBlockers();

  RendererExited(render_view_termination_status_, exit_code);

  delegate_->RenderViewGone(this,
                            static_cast<base::TerminationStatus>(status),
                            exit_code);
}

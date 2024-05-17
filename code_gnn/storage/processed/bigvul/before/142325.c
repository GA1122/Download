void ChromePasswordManagerClient::RenderFrameCreated(
    content::RenderFrameHost* render_frame_host) {
  AddToWidgetInputEventObservers(
      render_frame_host->GetView()->GetRenderWidgetHost(), this);
}

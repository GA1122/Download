void WebContentsImpl::DidChooseColorInColorChooser(SkColor color) {
  if (!color_chooser_info_.get())
    return;
  RenderFrameHost* rfh = RenderFrameHost::FromID(
      color_chooser_info_->render_process_id,
      color_chooser_info_->render_frame_id);
  if (!rfh)
    return;

  rfh->Send(new FrameMsg_DidChooseColorResponse(
      rfh->GetRoutingID(), color_chooser_info_->identifier, color));
}

void WebContentsImpl::OnOpenColorChooser(
    int color_chooser_id,
    SkColor color,
    const std::vector<ColorSuggestion>& suggestions) {
  if (!HasValidFrameSource())
    return;

  ColorChooser* new_color_chooser = delegate_ ?
      delegate_->OpenColorChooser(this, color, suggestions) :
      NULL;
  if (!new_color_chooser)
    return;
  if (color_chooser_info_.get())
    color_chooser_info_->chooser->End();

  color_chooser_info_.reset(new ColorChooserInfo(
      render_frame_message_source_->GetProcess()->GetID(),
      render_frame_message_source_->GetRoutingID(),
      new_color_chooser,
      color_chooser_id));
}

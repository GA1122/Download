void WebContentsImpl::OnOpenColorChooser(
    RenderFrameHostImpl* source,
    int color_chooser_id,
    SkColor color,
    const std::vector<ColorSuggestion>& suggestions) {
  ColorChooser* new_color_chooser = delegate_ ?
      delegate_->OpenColorChooser(this, color, suggestions) :
      NULL;
  if (!new_color_chooser)
    return;
  if (color_chooser_info_.get())
    color_chooser_info_->chooser->End();

  color_chooser_info_.reset(new ColorChooserInfo(
      source->GetProcess()->GetID(), source->GetRoutingID(), new_color_chooser,
      color_chooser_id));
}

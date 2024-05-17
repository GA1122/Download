void WebContentsImpl::OnSetSelectedColorInColorChooser(
    RenderFrameHostImpl* source,
    int color_chooser_id,
    SkColor color) {
  if (color_chooser_info_ &&
      color_chooser_info_->Matches(source, color_chooser_id))
    color_chooser_info_->chooser->SetSelectedColor(color);
}

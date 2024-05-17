void WebContentsImpl::OnSetSelectedColorInColorChooser(int color_chooser_id,
                                                       SkColor color) {
  if (color_chooser_info_ &&
      color_chooser_id == color_chooser_info_->identifier)
    color_chooser_info_->chooser->SetSelectedColor(color);
}

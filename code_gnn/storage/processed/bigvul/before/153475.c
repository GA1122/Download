bool TabStrip::ShouldDrawStrokes() const {
  if (!controller_->CanDrawStrokes())
    return false;

  return color_utils::GetContrastRatio(
             GetTabBackgroundColor(TAB_ACTIVE,
                                   BrowserNonClientFrameView::kActive),
             controller_->GetFrameColor(BrowserNonClientFrameView::kActive)) <
         1.3;
}

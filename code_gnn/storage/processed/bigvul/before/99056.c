void Section::Layout() {
  int x = 0;
  int y = 0;
  gfx::Size size = title_label_->GetPreferredSize();
  title_label_->SetBounds(x, y, size.width(), size.height());
  x += size.width() + kHGapTitleToSeparator;
#if defined(OS_WIN)
  separator_->SetBounds(x + kHExtraSeparatorPadding, y,
                        width() - x - 2 * kHExtraSeparatorPadding,
                        size.height());
#else
  NOTIMPLEMENTED();
#endif

  x = kHGapToBorder;
  y += title_label_->height() + kVGapTitleToImage;
  size = status_image_->GetPreferredSize();
  status_image_->SetBounds(x, y, size.width(), size.height());
  x += size.width() + kHGapImageToDescription;
  int w = width() - x;
  if (!head_line_label_->GetText().empty()) {
    size = head_line_label_->GetPreferredSize();
    head_line_label_->SetBounds(x, y, w > 0 ? w : 0, size.height());
    y += size.height() + kVGapHeadLineToDescription;
  } else {
    head_line_label_->SetBounds(x, y, 0, 0);
  }
  if (w > 0) {
    description_label_->SetBounds(x, y, w,
                                  description_label_->GetHeightForWidth(w));
  } else {
    description_label_->SetBounds(x, y, 0, 0);
  }
}

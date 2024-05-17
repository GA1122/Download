int Section::GetHeightForWidth(int width) {
  int height = 0;
  gfx::Size size = title_label_->GetPreferredSize();
  height += size.height() + kVGapTitleToImage;

  gfx::Size image_size = status_image_->GetPreferredSize();

  int text_height = 0;
  if (!head_line_label_->GetText().empty()) {
    size = head_line_label_->GetPreferredSize();
    text_height = size.height() + kVGapHeadLineToDescription;
  }

  int description_width =
      width - image_size.width() - kHGapImageToDescription - kHGapToBorder;
  text_height += description_label_->GetHeightForWidth(description_width);

  height += std::max(image_size.height(), text_height);

  return height;
}

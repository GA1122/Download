void Document::PageSizeAndMarginsInPixels(int page_index,
                                          DoubleSize& page_size,
                                          int& margin_top,
                                          int& margin_right,
                                          int& margin_bottom,
                                          int& margin_left) {
  scoped_refptr<ComputedStyle> style = StyleForPage(page_index);

  double width = page_size.Width();
  double height = page_size.Height();
  switch (style->PageSizeType()) {
    case EPageSizeType::kAuto:
      break;
    case EPageSizeType::kLandscape:
      if (width < height)
        std::swap(width, height);
      break;
    case EPageSizeType::kPortrait:
      if (width > height)
        std::swap(width, height);
      break;
    case EPageSizeType::kResolved: {
      FloatSize size = style->PageSize();
      width = size.Width();
      height = size.Height();
      break;
    }
    default:
      NOTREACHED();
  }
  page_size = DoubleSize(width, height);

  margin_top = style->MarginTop().IsAuto()
                   ? margin_top
                   : IntValueForLength(style->MarginTop(), width);
  margin_right = style->MarginRight().IsAuto()
                     ? margin_right
                     : IntValueForLength(style->MarginRight(), width);
  margin_bottom = style->MarginBottom().IsAuto()
                      ? margin_bottom
                      : IntValueForLength(style->MarginBottom(), width);
  margin_left = style->MarginLeft().IsAuto()
                    ? margin_left
                    : IntValueForLength(style->MarginLeft(), width);
}

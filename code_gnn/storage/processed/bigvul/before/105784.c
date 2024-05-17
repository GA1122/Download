gfx::Rect Label::GetTextBounds() const {
  gfx::Rect available_rect(GetAvailableRect());
  gfx::Size text_size(GetTextSize());
  text_size.set_width(std::min(available_rect.width(), text_size.width()));

  gfx::Insets insets = GetInsets();
  gfx::Point text_origin(insets.left(), insets.top());
  switch (horiz_alignment_) {
    case ALIGN_LEFT:
      break;
    case ALIGN_CENTER:
      text_origin.Offset((available_rect.width() + 1 - text_size.width()) / 2,
                         0);
      break;
    case ALIGN_RIGHT:
      text_origin.set_x(available_rect.right() - text_size.width());
      break;
    default:
      NOTREACHED();
      break;
  }
  text_origin.Offset(0,
      std::max(0, (available_rect.height() - text_size.height())) / 2);
  return gfx::Rect(text_origin, text_size);
}

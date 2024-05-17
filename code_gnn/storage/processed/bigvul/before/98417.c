gfx::Point ImageView::ComputeImageOrigin(const gfx::Size& image_size) const {
  gfx::Insets insets = GetInsets();

  int x;
  Alignment actual_horiz_alignment = horiz_alignment_;
  if (base::i18n::IsRTL() && (horiz_alignment_ != CENTER))
    actual_horiz_alignment = (horiz_alignment_ == LEADING) ? TRAILING : LEADING;
  switch (actual_horiz_alignment) {
    case LEADING:  x = insets.left();                                 break;
    case TRAILING: x = width() - insets.right() - image_size.width(); break;
    case CENTER:   x = (width() - image_size.width()) / 2;            break;
    default:       NOTREACHED(); x = 0;                               break;
  }

  int y;
  switch (vert_alignment_) {
    case LEADING:  y = insets.top();                                     break;
    case TRAILING: y = height() - insets.bottom() - image_size.height(); break;
    case CENTER:   y = (height() - image_size.height()) / 2;             break;
    default:       NOTREACHED(); y = 0;                                  break;
  }

  return gfx::Point(x, y);
}

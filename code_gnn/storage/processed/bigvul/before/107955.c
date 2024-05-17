int InfoBar::OffsetY(views::View* parent, const gfx::Size prefsize) {
  return CenterY(prefsize) -
      (static_cast<int>(target_height_) - parent->height());
}

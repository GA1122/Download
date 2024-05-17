int InfoBar::CenterY(const gfx::Size prefsize) {
  return std::max((static_cast<int>(target_height_) -
      prefsize.height()) / 2, 0);
}

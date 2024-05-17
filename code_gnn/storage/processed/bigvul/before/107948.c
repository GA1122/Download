void InfoBar::Layout() {
  gfx::Size button_ps = close_button_->GetPreferredSize();
  close_button_->SetBounds(width() - kHorizontalPadding - button_ps.width(),
                           OffsetY(this, button_ps), button_ps.width(),
                           button_ps.height());
}

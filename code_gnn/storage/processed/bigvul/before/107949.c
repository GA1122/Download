void AlertInfoBar::Layout() {
  InfoBar::Layout();

  gfx::Size icon_ps = icon_->GetPreferredSize();
  icon_->SetBounds(kHorizontalPadding, OffsetY(this, icon_ps), icon_ps.width(),
                   icon_ps.height());

  gfx::Size text_ps = label_->GetPreferredSize();
  int text_width = std::min(
      text_ps.width(),
      GetAvailableWidth() - icon_->bounds().right() - kIconLabelSpacing);
  label_->SetBounds(icon_->bounds().right() + kIconLabelSpacing,
                    OffsetY(this, text_ps), text_width, text_ps.height());
}

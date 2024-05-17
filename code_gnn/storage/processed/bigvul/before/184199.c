  void TranslateInfoBarBase::Layout() {
  InfoBar::Layout();
//   InfoBarView::Layout();
  
    gfx::Size icon_ps = icon_->GetPreferredSize();
  icon_->SetBounds(InfoBar::kHorizontalPadding, InfoBar::OffsetY(this, icon_ps),
      icon_ps.width(), icon_ps.height());
//   icon_->SetBounds(InfoBarView::kHorizontalPadding,
//       InfoBarView::OffsetY(this, icon_ps), icon_ps.width(), icon_ps.height());
  }
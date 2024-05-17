void SettingLevelBubbleView::Init(SkBitmap* icon, double level, bool enabled) {
  DCHECK(!icon_);
  DCHECK(icon);
  icon_ = icon;
  AddChildView(progress_bar_);
  progress_bar_->SetDisplayRange(0.0, 100.0);
  progress_bar_->EnableCanvasFlippingForRTLUI(true);
  EnableCanvasFlippingForRTLUI(true);
  SetLevel(level);
  SetEnabled(enabled);
}

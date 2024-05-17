void SettingLevelBubbleView::SetIcon(SkBitmap* icon) {
  DCHECK(icon);
  icon_ = icon;
  SchedulePaint();
}

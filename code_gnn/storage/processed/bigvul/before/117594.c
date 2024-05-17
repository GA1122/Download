void SettingLevelBubbleView::OnPaint(gfx::Canvas* canvas) {
  views::View::OnPaint(canvas);
  canvas->DrawBitmapInt(*icon_, kPadding, (height() - icon_->height()) / 2);
}

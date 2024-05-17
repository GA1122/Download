void ShelfWidget::SetPaintsBackground(ShelfBackgroundType background_type,
                                      AnimationChangeType change_type) {
  background_animator_.PaintBackground(background_type, change_type);
}

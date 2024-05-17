bool PictureLayer::UpdateCanUseLCDText() {
  if (!can_use_lcd_text_for_update_)
    return false;   
  if (can_use_lcd_text_for_update_ == can_use_lcd_text())
    return false;

  can_use_lcd_text_for_update_ = can_use_lcd_text();
  return true;
}

void TabStrip::SetBackgroundOffset(int offset) {
  for (int i = 0; i < tab_count(); ++i)
    tab_at(i)->set_background_offset(offset);
  new_tab_button_->set_background_offset(offset);
}

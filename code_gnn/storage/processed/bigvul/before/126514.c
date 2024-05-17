int TabStripGtk::GetMiniTabCount() const {
  int mini_count = 0;
  for (size_t i = 0; i < tab_data_.size(); ++i) {
    if (tab_data_[i].tab->mini())
      mini_count++;
    else
      return mini_count;
  }
  return mini_count;
}

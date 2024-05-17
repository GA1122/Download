int TabStripGtk::GetAvailableWidthForTabs(TabGtk* last_tab) const {
  if (!base::i18n::IsRTL())
    return last_tab->x() - bounds_.x() + last_tab->width();
  else
    return bounds_.width() - last_tab->x();
}

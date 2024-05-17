TabAlertState TabStrip::GetTabAlertState(int tab_index) const {
  return tab_at(tab_index)->data().alert_state;
}

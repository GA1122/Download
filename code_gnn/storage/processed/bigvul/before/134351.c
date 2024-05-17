int TabStrip::GetMiniTabCount() const {
  int mini_count = 0;
  while (mini_count < tab_count() && tab_at(mini_count)->data().mini)
    mini_count++;
  return mini_count;
}

int TabStrip::GenerateIdealBoundsForMiniTabs(int* first_non_mini_index) {
  int next_x = 0;
  int mini_width = Tab::GetMiniWidth();
  int tab_height = Tab::GetStandardSize().height();
  int index = 0;
  for (; index < tab_count() && tab_at(index)->data().mini; ++index) {
    set_ideal_bounds(index,
                     gfx::Rect(next_x, 0, mini_width, tab_height));
    next_x += mini_width + tab_h_offset();
  }
  if (index > 0 && index < tab_count())
    next_x += kMiniToNonMiniGap;
  if (first_non_mini_index)
    *first_non_mini_index = index;
  return next_x;
}

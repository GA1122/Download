void TabStrip::StartInsertTabAnimation(int model_index) {
  PrepareForAnimation();

  in_tab_close_ = false;
  available_width_for_tabs_ = -1;

  GenerateIdealBounds();

  Tab* tab = tab_at(model_index);
  if (model_index == 0) {
    tab->SetBounds(0, ideal_bounds(model_index).y(), 0,
                   ideal_bounds(model_index).height());
  } else {
    Tab* last_tab = tab_at(model_index - 1);
    tab->SetBounds(last_tab->bounds().right() + tab_h_offset(),
                   ideal_bounds(model_index).y(), 0,
                   ideal_bounds(model_index).height());
  }

  AnimateToIdealBounds();
}

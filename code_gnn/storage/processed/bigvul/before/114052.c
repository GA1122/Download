void TaskManagerView::InitAlwaysOnTopState() {
  is_always_on_top_ = false;
  if (GetSavedAlwaysOnTopState(&is_always_on_top_))
    GetWidget()->SetAlwaysOnTop(is_always_on_top_);
  AddAlwaysOnTopSystemMenuItem();
}

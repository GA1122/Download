void TabStrip::SetDropIndex(int tab_data_index, bool drop_before) {
  controller()->OnDropIndexUpdate(tab_data_index, drop_before);

  if (tab_data_index == -1) {
    if (drop_info_.get())
      drop_info_.reset(NULL);
    return;
  }

  if (drop_info_.get() && drop_info_->drop_index == tab_data_index &&
      drop_info_->drop_before == drop_before) {
    return;
  }

  bool is_beneath;
  gfx::Rect drop_bounds = GetDropBounds(tab_data_index, drop_before,
                                        &is_beneath);

  if (!drop_info_.get()) {
    drop_info_.reset(
        new DropInfo(tab_data_index, drop_before, !is_beneath, GetWidget()));
  } else {
    drop_info_->drop_index = tab_data_index;
    drop_info_->drop_before = drop_before;
    if (is_beneath == drop_info_->point_down) {
      drop_info_->point_down = !is_beneath;
      drop_info_->arrow_view->SetImage(
          GetDropArrowImage(drop_info_->point_down));
    }
  }

  drop_info_->arrow_window->SetBounds(drop_bounds);
  drop_info_->arrow_window->Show();
}

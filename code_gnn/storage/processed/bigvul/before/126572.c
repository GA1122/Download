void TabStripGtk::SetDropIndex(int index, bool drop_before) {
  bool is_beneath;
  gfx::Rect drop_bounds = GetDropBounds(index, drop_before, &is_beneath);

  if (index != -1 && !drop_before)
    hover_tab_selector_.StartTabTransition(index);
  else
    hover_tab_selector_.CancelTabTransition();

  if (!drop_info_.get()) {
    drop_info_.reset(new DropInfo(index, drop_before, !is_beneath));
  } else {
    if (!GTK_IS_WIDGET(drop_info_->container)) {
      drop_info_->CreateContainer();
    } else if (drop_info_->drop_index == index &&
               drop_info_->drop_before == drop_before) {
      return;
    }

    drop_info_->drop_index = index;
    drop_info_->drop_before = drop_before;
    if (is_beneath == drop_info_->point_down) {
      drop_info_->point_down = !is_beneath;
      drop_info_->drop_arrow = GetDropArrowImage(drop_info_->point_down);
    }
  }

  gtk_window_move(GTK_WINDOW(drop_info_->container),
                  drop_bounds.x(), drop_bounds.y());
  gtk_window_resize(GTK_WINDOW(drop_info_->container),
                    drop_bounds.width(), drop_bounds.height());
}

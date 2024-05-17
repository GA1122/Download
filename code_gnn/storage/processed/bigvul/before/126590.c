void TabStripGtk::TabMiniStateChanged(WebContents* contents, int index) {
  if (GetTabAt(index)->mini() == model_->IsMiniTab(index))
    return;

  GetTabAt(index)->set_mini(model_->IsMiniTab(index));
  if (window_ && window_->window() &&
      gtk_widget_get_visible(GTK_WIDGET(window_->window()))) {
    StartMiniTabAnimation(index);
  } else {
    Layout();
  }
}

void Browser::ShowInstant(TabContentsWrapper* preview_contents) {
  DCHECK(instant_->tab_contents() == GetSelectedTabContentsWrapper());
  window_->ShowInstant(preview_contents);
}

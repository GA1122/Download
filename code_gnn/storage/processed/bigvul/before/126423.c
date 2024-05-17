void BrowserWindowGtk::SetStarredState(bool is_starred) {
  toolbar_->GetLocationBarView()->SetStarred(is_starred);
}

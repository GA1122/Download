void BrowserWindowGtk::ZoomChangedForActiveTab(bool can_show_bubble) {
  toolbar_->GetLocationBarView()->ZoomChangedForActiveTab(
      can_show_bubble && !toolbar_->IsWrenchMenuShowing());
}

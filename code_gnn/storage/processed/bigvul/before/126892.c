void BrowserView::ZoomChangedForActiveTab(bool can_show_bubble) {
  GetLocationBarView()->ZoomChangedForActiveTab(
      can_show_bubble && !toolbar_->IsWrenchMenuShowing());
}

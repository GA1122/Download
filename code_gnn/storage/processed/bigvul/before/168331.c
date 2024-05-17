void BrowserView::OnWidgetMove() {
  if (!initialized_) {
    return;
  }


  if (status_bubble_.get())
    status_bubble_->Reposition();

  BookmarkBubbleView::Hide();

  LocationBarView* location_bar_view = GetLocationBarView();
  if (location_bar_view)
    location_bar_view->GetOmniboxView()->CloseOmniboxPopup();
}

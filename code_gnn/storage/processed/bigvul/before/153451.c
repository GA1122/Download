void TabStrip::OnViewIsDeleting(views::View* observed_view) {
  if (observed_view == hover_card_) {
    hover_card_->views::View::RemoveObserver(this);
    hover_card_event_sniffer_.reset();
    hover_card_ = nullptr;
  }
}

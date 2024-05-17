void CastDetailedView::UpdateReceiverListFromCachedData() {
  receiver_activity_map_.clear();
  scroll_content()->RemoveAllChildViews(true);

  for (auto& it : receivers_and_activities_) {
    const CastConfigDelegate::ReceiverAndActivity& receiver_activity =
        it.second;
    views::View* container = AddToReceiverList(receiver_activity);
    receiver_activity_map_[container] = it.first;
  }

  scroll_content()->SizeToPreferredSize();
  static_cast<views::View*>(scroller())->Layout();
}

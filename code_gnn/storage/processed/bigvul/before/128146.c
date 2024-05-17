void CastDetailedView::UpdateReceiverList() {
  cast_config_delegate_->GetReceiversAndActivities(base::Bind(
      &CastDetailedView::UpdateReceiverListCallback, base::Unretained(this)));
}

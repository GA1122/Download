views::View* TrayCast::CreateDetailedView(user::LoginStatus status) {
  Shell::GetInstance()->metrics()->RecordUserMetricsAction(
      ash::UMA_STATUS_AREA_DETAILED_CAST_VIEW);
  CHECK(detailed_ == nullptr);
  detailed_ = new tray::CastDetailedView(this, cast_config_delegate_, status);
  return detailed_;
}

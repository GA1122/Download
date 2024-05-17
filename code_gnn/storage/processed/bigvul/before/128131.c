views::View* TrayCast::CreateTrayView(user::LoginStatus status) {
  CHECK(tray_ == nullptr);
  tray_ = new tray::CastTrayView(this);
  tray_->SetVisible(is_casting_);
  return tray_;
}

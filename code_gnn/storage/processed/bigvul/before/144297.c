OobeUI* LoginDisplayHostWebUI::GetOobeUI() const {
  if (!login_view_)
    return nullptr;
  return login_view_->GetOobeUI();
}

void PasswordAccessoryControllerImpl::DidNavigateMainFrame() {
  favicon_tracker_.TryCancelAll();   
  current_origin_ = url::Origin();
  icons_request_data_.clear();
  origin_suggestions_.clear();
}

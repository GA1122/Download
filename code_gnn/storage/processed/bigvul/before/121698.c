void MediaStreamDevicesController::Deny(bool update_content_setting) {
  if (content_settings_) {
    content_settings_->OnContentBlocked(CONTENT_SETTINGS_TYPE_MEDIASTREAM,
                                        std::string());
  }

  NotifyUIRequestDenied();

  if (update_content_setting)
    SetPermission(false);

  content::MediaResponseCallback cb = callback_;
  callback_.Reset();
  cb.Run(content::MediaStreamDevices(), scoped_ptr<content::MediaStreamUI>());
}

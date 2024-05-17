void FaviconSource::OnFaviconDataAvailable(
    FaviconService::Handle request_handle,
    history::FaviconData favicon) {
  FaviconService* favicon_service =
      profile_->GetFaviconService(Profile::EXPLICIT_ACCESS);
  int request_id = cancelable_consumer_.GetClientData(favicon_service,
                                                      request_handle);

  if (favicon.is_valid()) {
    SendResponse(request_id, favicon.image_data);
  } else {
    SendDefaultResponse(request_id);
  }
 }

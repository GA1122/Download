void GetReleaseTrackCallback(AutomationJSONReply* reply,
                             const std::string& track) {
  if (track.empty()) {
    reply->SendError("Unable to get release track.");
    delete reply;
    return;
  }

  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  return_value->SetString("release_track", track);

  const UpdateEngineClient::Status& status =
      DBusThreadManager::Get()->GetUpdateEngineClient()->GetLastStatus();
  UpdateEngineClient::UpdateStatusOperation update_status =
      status.status;
  return_value->SetString("status", UpdateStatusToString(update_status));
  if (update_status == UpdateEngineClient::UPDATE_STATUS_DOWNLOADING)
    return_value->SetDouble("download_progress", status.download_progress);
  if (status.last_checked_time > 0)
    return_value->SetInteger("last_checked_time", status.last_checked_time);
  if (status.new_size > 0)
    return_value->SetInteger("new_size", status.new_size);

  reply->SendSuccess(return_value.get());
  delete reply;
}

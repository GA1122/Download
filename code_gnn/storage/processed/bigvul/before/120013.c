    CompleteGetFileProperties(drive::FileError error) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(0 <= processed_count_ && processed_count_ < properties_list_.size());

  processed_count_++;
  if (processed_count_ < properties_list_.size())
    return;

  results_ = extensions::api::file_browser_private::GetDriveEntryProperties::
      Results::Create(properties_list_);
  SendResponse(true);
}

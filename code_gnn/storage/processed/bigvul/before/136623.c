void DocumentLoader::ProcessData(const char* data, size_t length) {
  application_cache_host_->MainResourceDataReceived(data, length);
  time_of_last_data_received_ = CurrentTimeTicks();

  if (IsArchiveMIMEType(GetResponse().MimeType()))
    return;
  CommitData(data, length);

  if (frame_ && frame_->GetDocument()->IsMediaDocument())
    fetcher_->StopFetching();
}

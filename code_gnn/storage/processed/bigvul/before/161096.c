std::string MediaStreamManager::AddRequest(DeviceRequest* request) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  std::string unique_label;
  do {
    unique_label = RandomLabel();
  } while (FindRequest(unique_label) != nullptr);

  requests_.push_back(std::make_pair(unique_label, request));

  return unique_label;
}

void FakeCrosDisksClient::NotifyFormatCompleted(
    FormatError error_code,
    const std::string& device_path) {
  for (auto& observer : observer_list_)
    observer.OnFormatCompleted(error_code, device_path);
}

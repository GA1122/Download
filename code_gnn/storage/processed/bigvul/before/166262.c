bool VideoCaptureManager::GetDeviceFormatsInUse(
    media::VideoCaptureSessionId capture_session_id,
    media::VideoCaptureFormats* formats_in_use) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(formats_in_use->empty());

  auto it = sessions_.find(capture_session_id);
  if (it == sessions_.end())
    return false;
  std::ostringstream string_stream;
  string_stream << "GetDeviceFormatsInUse for device: " << it->second.name;
  EmitLogMessage(string_stream.str(), 1);

  base::Optional<media::VideoCaptureFormat> format =
      GetDeviceFormatInUse(it->second.type, it->second.id);
  if (format.has_value())
    formats_in_use->push_back(format.value());

  return true;
}

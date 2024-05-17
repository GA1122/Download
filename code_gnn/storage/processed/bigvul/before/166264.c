bool VideoCaptureManager::GetDeviceSupportedFormats(
    media::VideoCaptureSessionId capture_session_id,
    media::VideoCaptureFormats* supported_formats) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(supported_formats->empty());

  auto it = sessions_.find(capture_session_id);
  if (it == sessions_.end())
    return false;
  std::ostringstream string_stream;
  string_stream << "GetDeviceSupportedFormats for device: " << it->second.name;
  EmitLogMessage(string_stream.str(), 1);

  return GetDeviceSupportedFormats(it->second.id, supported_formats);
}

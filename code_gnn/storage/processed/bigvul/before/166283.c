int VideoCaptureManager::Open(const MediaStreamDevice& device) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  const media::VideoCaptureSessionId capture_session_id =
      new_capture_session_id_++;

  DCHECK(sessions_.find(capture_session_id) == sessions_.end());
  std::ostringstream string_stream;
  string_stream << "VideoCaptureManager::Open, device.name = " << device.name
                << ", device.id = " << device.id
                << ", capture_session_id = " << capture_session_id;
  EmitLogMessage(string_stream.str(), 1);

  sessions_[capture_session_id] = device;

  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(&VideoCaptureManager::OnOpened, this,
                                device.type, capture_session_id));
  return capture_session_id;
}

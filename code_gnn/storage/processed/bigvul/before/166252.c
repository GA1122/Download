void VideoCaptureManager::Close(int capture_session_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  std::ostringstream string_stream;
  string_stream << "VideoCaptureManager::Close, capture_session_id = "
                << capture_session_id;
  EmitLogMessage(string_stream.str(), 1);

  auto session_it = sessions_.find(capture_session_id);
  if (session_it == sessions_.end()) {
    return;
  }

  VideoCaptureController* const existing_device =
      LookupControllerByMediaTypeAndDeviceId(session_it->second.type,
                                             session_it->second.id);
  if (existing_device) {
    existing_device->StopSession(capture_session_id);

    DestroyControllerIfNoClients(existing_device);
  }

  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(&VideoCaptureManager::OnClosed, this,
                                session_it->second.type, capture_session_id));
  sessions_.erase(session_it);
}

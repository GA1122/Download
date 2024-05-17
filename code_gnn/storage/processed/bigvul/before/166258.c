void VideoCaptureManager::EnumerateDevices(
    EnumerationCallback client_callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  EmitLogMessage("VideoCaptureManager::EnumerateDevices", 1);

  video_capture_provider_->GetDeviceInfosAsync(
      media::BindToCurrentLoop(base::BindOnce(
          &VideoCaptureManager::OnDeviceInfosReceived, this,
          base::Owned(new base::ElapsedTimer()), std::move(client_callback))));
}

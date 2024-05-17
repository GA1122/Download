void MediaStreamManager::InitializeMaybeAsync(
    std::unique_ptr<VideoCaptureProvider> video_capture_provider) {
  if (!BrowserThread::CurrentlyOn(BrowserThread::IO)) {
    BrowserThread::PostTask(
        BrowserThread::IO, FROM_HERE,
        base::BindOnce(&MediaStreamManager::InitializeMaybeAsync,
                       base::Unretained(this),
                       std::move(video_capture_provider)));
    return;
  }

  g_media_stream_manager_tls_ptr.Pointer()->Set(this);

  audio_input_device_manager_ = new AudioInputDeviceManager(audio_system_);
  audio_input_device_manager_->RegisterListener(this);

  base::MessageLoop::current()->AddDestructionObserver(this);

  video_capture_manager_ =
      new VideoCaptureManager(std::move(video_capture_provider),
                              base::BindRepeating(&SendVideoCaptureLogMessage));
  video_capture_manager_->RegisterListener(this);

  media_devices_manager_.reset(
      new MediaDevicesManager(audio_system_, video_capture_manager_, this));
}

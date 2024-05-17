MediaStreamManager::MediaStreamManager(
    media::AudioSystem* audio_system,
    scoped_refptr<base::SingleThreadTaskRunner> audio_task_runner,
    std::unique_ptr<VideoCaptureProvider> video_capture_provider)
    : audio_system_(audio_system),
      fake_ui_factory_() {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kUseFakeUIForMediaStream)) {
    fake_ui_factory_ = base::Bind([] {
      return std::make_unique<FakeMediaStreamUIProxy>(
           false);
    });
  }

  DCHECK(audio_system_);

  if (!video_capture_provider) {
    scoped_refptr<base::SingleThreadTaskRunner> device_task_runner =
#if defined(OS_WIN)
        nullptr;
#else
        std::move(audio_task_runner);
#endif

    if (!device_task_runner) {
      video_capture_thread_.emplace("VideoCaptureThread");
#if defined(OS_WIN)
      video_capture_thread_->init_com_with_mta(false);
#endif
      CHECK(video_capture_thread_->Start());
      device_task_runner = video_capture_thread_->task_runner();
    }

#if defined(OS_CHROMEOS)
    if (media::ShouldUseCrosCameraService()) {
      media::VideoCaptureDeviceFactoryChromeOS::SetGpuBufferManager(
          GpuMemoryBufferManagerSingleton::GetInstance());
      media::CameraHalDispatcherImpl::GetInstance()->Start(
          base::BindRepeating(
              &VideoCaptureDependencies::CreateJpegDecodeAccelerator),
          base::BindRepeating(
              &VideoCaptureDependencies::CreateJpegEncodeAccelerator));
    }
#endif

    if (base::FeatureList::IsEnabled(features::kMojoVideoCapture)) {
      video_capture_provider = std::make_unique<VideoCaptureProviderSwitcher>(
          std::make_unique<ServiceVideoCaptureProvider>(
              base::BindRepeating(&SendVideoCaptureLogMessage)),
          InProcessVideoCaptureProvider::CreateInstanceForNonDeviceCapture(
              std::move(device_task_runner),
              base::BindRepeating(&SendVideoCaptureLogMessage)));
    } else {
      video_capture::uma::LogVideoCaptureServiceEvent(
          video_capture::uma::BROWSER_USING_LEGACY_CAPTURE);
      video_capture_provider = InProcessVideoCaptureProvider::CreateInstance(
          std::make_unique<media::VideoCaptureSystemImpl>(
              media::CreateVideoCaptureDeviceFactory(
                  base::CreateSingleThreadTaskRunnerWithTraits(
                      {BrowserThread::UI}))),
          std::move(device_task_runner),
          base::BindRepeating(&SendVideoCaptureLogMessage));
    }
  }
  InitializeMaybeAsync(std::move(video_capture_provider));

  if (ServiceManagerConnection::GetForProcess()) {
    audio_service_listener_ = std::make_unique<AudioServiceListener>(
        ServiceManagerConnection::GetForProcess()->GetConnector()->Clone());
  }

  base::PowerMonitor* power_monitor = base::PowerMonitor::Get();
  if (power_monitor)
    power_monitor->AddObserver(this);
}

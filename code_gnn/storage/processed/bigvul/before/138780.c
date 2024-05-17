void RenderFrameHostImpl::RegisterMojoInterfaces() {
  device::GeolocationServiceContext* geolocation_service_context =
      delegate_ ? delegate_->GetGeolocationServiceContext() : NULL;

#if !defined(OS_ANDROID)
  GetInterfaceRegistry()->AddInterface(
      base::Bind(&InstalledAppProviderImplDefault::Create));
#endif   

  if (geolocation_service_context) {
    GetInterfaceRegistry()->AddInterface(
        base::Bind(&device::GeolocationServiceContext::CreateService,
                   base::Unretained(geolocation_service_context)));
  }

  device::mojom::WakeLockContext* wake_lock_service_context =
      delegate_ ? delegate_->GetWakeLockServiceContext() : nullptr;
  if (wake_lock_service_context) {
    GetInterfaceRegistry()->AddInterface<device::mojom::WakeLockService>(
        base::Bind(&device::mojom::WakeLockContext::GetWakeLock,
                   base::Unretained(wake_lock_service_context)));
  }

  if (!permission_service_context_)
    permission_service_context_.reset(new PermissionServiceContext(this));

  GetInterfaceRegistry()->AddInterface(
      base::Bind(&PermissionServiceContext::CreateService,
                 base::Unretained(permission_service_context_.get())));

  GetInterfaceRegistry()->AddInterface(base::Bind(
      &PresentationServiceImpl::CreateMojoService, base::Unretained(this)));

  GetInterfaceRegistry()->AddInterface(
      base::Bind(&MediaSessionServiceImpl::Create, base::Unretained(this)));

#if defined(OS_ANDROID)
  GetInterfaceRegistry()->AddInterface<media::mojom::Renderer>(base::Bind(
      &content::CreateMediaPlayerRenderer, base::Unretained(this)));
#endif   

  GetInterfaceRegistry()->AddInterface(base::Bind(
      base::IgnoreResult(&RenderFrameHostImpl::CreateWebBluetoothService),
      base::Unretained(this)));

  GetInterfaceRegistry()->AddInterface<media::mojom::InterfaceFactory>(this);

  GetInterfaceRegistry()->AddInterface(
      base::Bind(&WebSocketManager::CreateWebSocket,
                 process_->GetID(),
                 routing_id_));

#if BUILDFLAG(ENABLE_VR)
  GetInterfaceRegistry()->AddInterface<device::mojom::VRService>(
      base::Bind(&device::VRServiceImpl::Create));
#else
  GetInterfaceRegistry()->AddInterface<device::mojom::VRService>(
      base::Bind(&IgnoreInterfaceRequest<device::mojom::VRService>));
#endif

#if BUILDFLAG(ENABLE_WEBRTC)
  if (BrowserMainLoop::GetInstance()) {
    MediaStreamManager* media_stream_manager =
        BrowserMainLoop::GetInstance()->media_stream_manager();
    GetInterfaceRegistry()->AddInterface(
        base::Bind(&MediaDevicesDispatcherHost::Create, GetProcess()->GetID(),
                   GetRoutingID(), GetProcess()
                                       ->GetBrowserContext()
                                       ->GetMediaDeviceIDSalt(),
                   base::Unretained(media_stream_manager)),
        BrowserThread::GetTaskRunnerForThread(BrowserThread::IO));
  }
#endif

#if BUILDFLAG(ENABLE_MEDIA_REMOTING)
  GetInterfaceRegistry()->AddInterface(base::Bind(
      &RemoterFactoryImpl::Bind, GetProcess()->GetID(), GetRoutingID()));
#endif   

  GetContentClient()->browser()->RegisterRenderFrameMojoInterfaces(
      GetInterfaceRegistry(), this);
}

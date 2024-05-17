void RenderProcessHostImpl::RegisterMojoInterfaces() {
  auto registry = base::MakeUnique<service_manager::InterfaceRegistry>(
      service_manager::mojom::kServiceManager_ConnectorSpec);

  channel_->AddAssociatedInterfaceForIOThread(
      base::Bind(&IndexedDBDispatcherHost::AddBinding, indexed_db_factory_));

#if defined(OS_ANDROID)
  AddUIThreadInterface(registry.get(),
                       GetGlobalJavaInterfaces()
                           ->CreateInterfaceFactory<device::BatteryMonitor>());
  AddUIThreadInterface(
      registry.get(), GetGlobalJavaInterfaces()
                          ->CreateInterfaceFactory<
                              shape_detection::mojom::FaceDetectionProvider>());
  AddUIThreadInterface(
      registry.get(),
      GetGlobalJavaInterfaces()
          ->CreateInterfaceFactory<shape_detection::mojom::BarcodeDetection>());
  AddUIThreadInterface(
      registry.get(),
      GetGlobalJavaInterfaces()
          ->CreateInterfaceFactory<shape_detection::mojom::TextDetection>());
#else
  AddUIThreadInterface(
      registry.get(), base::Bind(&device::BatteryMonitorImpl::Create));
  AddUIThreadInterface(
      registry.get(),
      base::Bind(&ForwardShapeDetectionRequest<
                 shape_detection::mojom::BarcodeDetectionRequest>));
  AddUIThreadInterface(
      registry.get(),
      base::Bind(&ForwardShapeDetectionRequest<
                 shape_detection::mojom::FaceDetectionProviderRequest>));
#endif
  AddUIThreadInterface(
      registry.get(),
      base::Bind(&PermissionServiceContext::CreateService,
                 base::Unretained(permission_service_context_.get())));
  AddUIThreadInterface(registry.get(), base::Bind(&ImageCaptureImpl::Create));

  AddUIThreadInterface(
      registry.get(),
      base::Bind(&PaymentAppContextImpl::CreatePaymentAppManager,
                 base::Unretained(
                     storage_partition_impl_->GetPaymentAppContext())));

  AddUIThreadInterface(
      registry.get(),
      base::Bind(&RenderProcessHostImpl::
                     CreateOffscreenCanvasCompositorFrameSinkProvider,
                 base::Unretained(this)));

  AddUIThreadInterface(registry.get(),
                       base::Bind(&OffscreenCanvasSurfaceFactoryImpl::Create));
  AddUIThreadInterface(
      registry.get(),
      base::Bind(&BackgroundSyncContext::CreateService,
                 base::Unretained(
                     storage_partition_impl_->GetBackgroundSyncContext())));
  AddUIThreadInterface(
      registry.get(),
      base::Bind(&PlatformNotificationContextImpl::CreateService,
                 base::Unretained(
                     storage_partition_impl_->GetPlatformNotificationContext()),
                 GetID()));
  AddUIThreadInterface(
      registry.get(),
      base::Bind(&RenderProcessHostImpl::CreateStoragePartitionService,
                 base::Unretained(this)));
  AddUIThreadInterface(
      registry.get(),
      base::Bind(&BroadcastChannelProvider::Connect,
                 base::Unretained(
                     storage_partition_impl_->GetBroadcastChannelProvider())));
  if (base::FeatureList::IsEnabled(features::kMemoryCoordinator)) {
    AddUIThreadInterface(
        registry.get(), base::Bind(&CreateMemoryCoordinatorHandle, GetID()));
  }

  scoped_refptr<base::SingleThreadTaskRunner> file_task_runner =
      BrowserThread::GetTaskRunnerForThread(BrowserThread::FILE);
  registry->AddInterface(base::Bind(&MimeRegistryImpl::Create),
                         file_task_runner);
#if BUILDFLAG(USE_MINIKIN_HYPHENATION)
  registry->AddInterface(base::Bind(&hyphenation::HyphenationImpl::Create),
                         file_task_runner);
#endif

#if defined(OS_ANDROID)
  AddUIThreadInterface(registry.get(),
                       base::Bind(&device::DeviceLightHost::Create));
  AddUIThreadInterface(registry.get(),
                       base::Bind(&device::DeviceMotionHost::Create));
  AddUIThreadInterface(registry.get(),
                       base::Bind(&device::DeviceOrientationHost::Create));
  AddUIThreadInterface(
      registry.get(),
      base::Bind(&device::DeviceOrientationAbsoluteHost::Create));
#else
  registry->AddInterface(base::Bind(&device::DeviceLightHost::Create));
  registry->AddInterface(base::Bind(&device::DeviceMotionHost::Create));
  registry->AddInterface(base::Bind(&device::DeviceOrientationHost::Create));
  registry->AddInterface(
      base::Bind(&device::DeviceOrientationAbsoluteHost::Create));
#endif   

  registry->AddInterface(base::Bind(&device::GamepadMonitor::Create));

  registry->AddInterface(base::Bind(&RenderProcessHostImpl::CreateMusGpuRequest,
                                    base::Unretained(this)));

  registry->AddInterface(
      base::Bind(&VideoCaptureHost::Create,
                 BrowserMainLoop::GetInstance()->media_stream_manager()));

  AddUIThreadInterface(
      registry.get(), base::Bind(&WebSocketManager::CreateWebSocket, GetID(),
                                 MSG_ROUTING_NONE));

  if (!service_manager::ServiceManagerIsRemote()) {
    discardable_memory::DiscardableSharedMemoryManager* manager =
        BrowserMainLoop::GetInstance()->discardable_shared_memory_manager();
    registry->AddInterface(
        base::Bind(&discardable_memory::DiscardableSharedMemoryManager::Bind,
                   base::Unretained(manager)));
  }

  GetContentClient()->browser()->ExposeInterfacesToRenderer(registry.get(),
                                                            this);

  ServiceManagerConnection* service_manager_connection =
      BrowserContext::GetServiceManagerConnectionFor(browser_context_);
  std::unique_ptr<ConnectionFilterImpl> connection_filter(
      new ConnectionFilterImpl(child_connection_->child_identity(),
                               std::move(registry)));
  connection_filter_controller_ = connection_filter->controller();
  connection_filter_id_ = service_manager_connection->AddConnectionFilter(
      std::move(connection_filter));
}

 void MediaInterfaceProxy::ConnectToService() {
// void MediaInterfaceProxy::OnCdmServiceConnectionError() {
    DVLOG(1) << __FUNCTION__;
    DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(!interface_factory_ptr_);
  
//   cdm_interface_factory_ptr_.reset();
// }
// 
// service_manager::mojom::InterfaceProviderPtr
// MediaInterfaceProxy::GetFrameServices() {
    service_manager::mojom::InterfaceProviderPtr interfaces;
// 
    auto provider = base::MakeUnique<media::MediaInterfaceProvider>(
        mojo::MakeRequest(&interfaces));
// 
  #if BUILDFLAG(ENABLE_MOJO_CDM)
    net::URLRequestContextGetter* context_getter =
       BrowserContext::GetDefaultStoragePartition(
           render_frame_host_->GetProcess()->GetBrowserContext())
           ->GetURLRequestContext();
    provider->registry()->AddInterface(base::Bind(
        &ProvisionFetcherImpl::Create, base::RetainedRef(context_getter)));
  #endif   
// 
    GetContentClient()->browser()->ExposeInterfacesToMediaService(
        provider->registry(), render_frame_host_);
  
    media_registries_.push_back(std::move(provider));
  
//   return interfaces;
// }
// 
// void MediaInterfaceProxy::ConnectToMediaService() {
//   DVLOG(1) << __FUNCTION__;
// 
    media::mojom::MediaServicePtr media_service;
// 
//    
    service_manager::Connector* connector =
        ServiceManagerConnection::GetForProcess()->GetConnector();
    connector->BindInterface(media::mojom::kMediaServiceName, &media_service);
// 
    media_service->CreateInterfaceFactory(MakeRequest(&interface_factory_ptr_),
                                        std::move(interfaces));
  interface_factory_ptr_.set_connection_error_handler(base::Bind(
      &MediaInterfaceProxy::OnConnectionError, base::Unretained(this)));
//                                         GetFrameServices());
// 
//   interface_factory_ptr_.set_connection_error_handler(
//       base::Bind(&MediaInterfaceProxy::OnMediaServiceConnectionError,
//                  base::Unretained(this)));
// }
// 
// void MediaInterfaceProxy::ConnectToCdmService() {
//   DVLOG(1) << __FUNCTION__;
// 
//   media::mojom::MediaServicePtr media_service;
// 
//    
//   service_manager::Connector* connector =
//       ServiceManagerConnection::GetForProcess()->GetConnector();
//   connector->BindInterface(media::mojom::kCdmServiceName, &media_service);
// 
//   media_service->CreateInterfaceFactory(
//       MakeRequest(&cdm_interface_factory_ptr_), GetFrameServices());
// 
//   cdm_interface_factory_ptr_.set_connection_error_handler(
//       base::Bind(&MediaInterfaceProxy::OnCdmServiceConnectionError,
//                  base::Unretained(this)));
  }
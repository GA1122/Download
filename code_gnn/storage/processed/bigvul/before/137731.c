void ChromeContentUtilityClient::RegisterServices(
    ChromeContentUtilityClient::StaticServiceMap* services) {
#if BUILDFLAG(ENABLE_PRINTING)
  service_manager::EmbeddedServiceInfo pdf_compositor_info;
  pdf_compositor_info.factory =
      base::Bind(&printing::CreatePdfCompositorService, GetUserAgent());
  services->emplace(printing::mojom::kServiceName, pdf_compositor_info);
#endif

#if BUILDFLAG(ENABLE_PRINT_PREVIEW)
  service_manager::EmbeddedServiceInfo pdf_to_pwg_converter_info;
  pdf_to_pwg_converter_info.factory =
      base::Bind(&printing::PDFToPWGRasterConverterService::CreateService);
  services->emplace(printing::mojom::kPdfToPwgRasterConverterServiceName,
                    pdf_to_pwg_converter_info);
#endif

  service_manager::EmbeddedServiceInfo profiling_info;
  profiling_info.task_runner = content::ChildThread::Get()->GetIOTaskRunner();
  profiling_info.factory =
      base::Bind(&profiling::ProfilingService::CreateService);
  services->emplace(profiling::mojom::kServiceName, profiling_info);

#if !defined(OS_ANDROID)
  service_manager::EmbeddedServiceInfo proxy_resolver_info;
  proxy_resolver_info.task_runner =
      content::ChildThread::Get()->GetIOTaskRunner();
  proxy_resolver_info.factory =
      base::Bind(&proxy_resolver::ProxyResolverService::CreateService);
  services->emplace(proxy_resolver::mojom::kProxyResolverServiceName,
                    proxy_resolver_info);

  service_manager::EmbeddedServiceInfo profile_import_info;
  profile_import_info.factory =
      base::Bind(&ProfileImportService::CreateService);
  services->emplace(chrome::mojom::kProfileImportServiceName,
                    profile_import_info);
#endif

#if BUILDFLAG(ENABLE_PACKAGE_MASH_SERVICES)
  RegisterMashServices(services);
#endif
}

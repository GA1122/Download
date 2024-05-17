ChromeContentBrowserClient::GetExtraServiceManifests() {
  return std::vector<content::ContentBrowserClient::ServiceManifestInfo>({
#if BUILDFLAG(ENABLE_NACL)
    {nacl::kNaClLoaderServiceName, IDR_NACL_LOADER_MANIFEST},
#if defined(OS_WIN)
        {nacl::kNaClBrokerServiceName, IDR_NACL_BROKER_MANIFEST},
#endif   
#endif   
#if BUILDFLAG(ENABLE_PRINTING)
        {printing::mojom::kServiceName, IDR_PDF_COMPOSITOR_MANIFEST},
#endif
        {chrome::mojom::kRendererServiceName,
         IDR_CHROME_RENDERER_SERVICE_MANIFEST},
  });
}

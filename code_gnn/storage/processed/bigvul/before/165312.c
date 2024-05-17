void ChromeContentBrowserClient::RegisterOutOfProcessServices(
    OutOfProcessServiceMap* services) {
#if BUILDFLAG(ENABLE_ISOLATED_XR_SERVICE)
  (*services)[device::mojom::kVrIsolatedServiceName] = base::BindRepeating(
      &l10n_util::GetStringUTF16, IDS_ISOLATED_XR_PROCESS_NAME);
#endif

#if BUILDFLAG(ENABLE_PRINTING)
  (*services)[printing::mojom::kServiceName] =
      base::BindRepeating(&l10n_util::GetStringUTF16,
                          IDS_UTILITY_PROCESS_PDF_COMPOSITOR_SERVICE_NAME);
#endif

#if BUILDFLAG(ENABLE_PRINT_PREVIEW) || \
    (BUILDFLAG(ENABLE_PRINTING) && defined(OS_WIN))
  (*services)[printing::mojom::kChromePrintingServiceName] =
      base::BindRepeating(&l10n_util::GetStringUTF16,
                          IDS_UTILITY_PROCESS_PRINTING_SERVICE_NAME);
#endif

  (*services)[heap_profiling::mojom::kServiceName] = base::BindRepeating(
      &l10n_util::GetStringUTF16, IDS_UTILITY_PROCESS_PROFILING_SERVICE_NAME);

#if BUILDFLAG(ENABLE_EXTENSIONS) || defined(OS_ANDROID)
  (*services)[chrome::mojom::kMediaGalleryUtilServiceName] =
      base::BindRepeating(&l10n_util::GetStringUTF16,
                          IDS_UTILITY_PROCESS_MEDIA_GALLERY_UTILITY_NAME);
#endif

#if BUILDFLAG(ENABLE_EXTENSIONS)
  (*services)[chrome::mojom::kRemovableStorageWriterServiceName] =
      base::BindRepeating(&l10n_util::GetStringUTF16,
                          IDS_UTILITY_PROCESS_IMAGE_WRITER_NAME);
#endif

#if defined(OS_WIN)
  (*services)[chrome::mojom::kUtilWinServiceName] = base::BindRepeating(
      &l10n_util::GetStringUTF16, IDS_UTILITY_PROCESS_UTILITY_WIN_NAME);
#endif

#if defined(OS_WIN) && BUILDFLAG(ENABLE_EXTENSIONS)
  (*services)[chrome::mojom::kWifiUtilWinServiceName] =
      base::BindRepeating(&l10n_util::GetStringUTF16,
                          IDS_UTILITY_PROCESS_WIFI_CREDENTIALS_GETTER_NAME);
#endif

#if !defined(OS_ANDROID)
  (*services)[chrome::mojom::kProfileImportServiceName] = base::BindRepeating(
      &l10n_util::GetStringUTF16, IDS_UTILITY_PROCESS_PROFILE_IMPORTER_NAME);

  (*services)[proxy_resolver::mojom::kProxyResolverServiceName] =
      base::BindRepeating(&l10n_util::GetStringUTF16,
                          IDS_UTILITY_PROCESS_PROXY_RESOLVER_NAME);
#endif

#if BUILDFLAG(ENABLE_PRINTING) && defined(OS_CHROMEOS)
  (*services)[chrome::mojom::kCupsIppParserServiceName] =
      base::BindRepeating(&l10n_util::GetStringUTF16,
                          IDS_UTILITY_PROCESS_CUPS_IPP_PARSER_SERVICE_NAME);
#endif

#if defined(FULL_SAFE_BROWSING) || defined(OS_CHROMEOS)
  (*services)[chrome::mojom::kFileUtilServiceName] = base::BindRepeating(
      &l10n_util::GetStringUTF16, IDS_UTILITY_PROCESS_FILE_UTILITY_NAME);
#endif

#if !defined(OS_ANDROID)
  (*services)[patch::mojom::kServiceName] = base::BindRepeating(
      &l10n_util::GetStringUTF16, IDS_UTILITY_PROCESS_PATCH_NAME);
#endif

  (*services)[unzip::mojom::kServiceName] = base::BindRepeating(
      &l10n_util::GetStringUTF16, IDS_UTILITY_PROCESS_UNZIP_NAME);

#if defined(OS_CHROMEOS)
  ash_service_registry::RegisterOutOfProcessServices(services);

  (*services)[chromeos::ime::mojom::kServiceName] = base::BindRepeating(
      &l10n_util::GetStringUTF16, IDS_UTILITY_PROCESS_IME_SERVICE_NAME);
#endif

#if BUILDFLAG(ENABLE_SIMPLE_BROWSER_SERVICE_OUT_OF_PROCESS)
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kLaunchSimpleBrowserSwitch)) {
    (*services)[simple_browser::mojom::kServiceName] =
        base::BindRepeating([]() -> base::string16 {
          return base::ASCIIToUTF16("Simple Browser");
        });
  }
#endif

#if !defined(OS_ANDROID)
  if (base::FeatureList::IsEnabled(mirroring::features::kMirroringService) &&
      base::FeatureList::IsEnabled(features::kAudioServiceAudioStreams) &&
      base::FeatureList::IsEnabled(network::features::kNetworkService)) {
    (*services)[mirroring::mojom::kServiceName] =
        base::BindRepeating(&base::ASCIIToUTF16, "Mirroring Service");
  }
#endif
}

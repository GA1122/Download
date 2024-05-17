ChromeContentBrowserClient::ChromeContentBrowserClient(
    ChromeFeatureListCreator* chrome_feature_list_creator)
    : chrome_feature_list_creator_(chrome_feature_list_creator),
      weak_factory_(this) {
#if BUILDFLAG(ENABLE_PLUGINS)
  for (size_t i = 0; i < arraysize(kPredefinedAllowedDevChannelOrigins); ++i)
    allowed_dev_channel_origins_.insert(kPredefinedAllowedDevChannelOrigins[i]);
  for (size_t i = 0; i < arraysize(kPredefinedAllowedFileHandleOrigins); ++i)
    allowed_file_handle_origins_.insert(kPredefinedAllowedFileHandleOrigins[i]);
  for (size_t i = 0; i < arraysize(kPredefinedAllowedSocketOrigins); ++i)
    allowed_socket_origins_.insert(kPredefinedAllowedSocketOrigins[i]);

  extra_parts_.push_back(new ChromeContentBrowserClientPluginsPart);
#endif

#if !defined(OS_ANDROID)
  TtsExtensionEngine* tts_extension_engine = TtsExtensionEngine::GetInstance();
  TtsController::GetInstance()->SetTtsEngineDelegate(tts_extension_engine);
#endif

#if defined(OS_CHROMEOS)
  extra_parts_.push_back(new ChromeContentBrowserClientChromeOsPart);
#endif   

#if BUILDFLAG(ENABLE_EXTENSIONS)
  extra_parts_.push_back(new ChromeContentBrowserClientExtensionsPart);
#endif

  gpu_binder_registry_.AddInterface(
      base::Bind(&metrics::CallStackProfileCollector::Create));
}

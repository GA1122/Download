void ChromeContentBrowserClient::RegisterInProcessServices(
    StaticServiceMap* services,
    content::ServiceManagerConnection* connection) {
  connection->AddServiceRequestHandler(
      chrome::mojom::kServiceName,
      ChromeService::GetInstance()->CreateChromeServiceRequestHandler());

  if (!g_browser_process || g_browser_process->pref_service_factory()) {
    service_manager::EmbeddedServiceInfo info;
    info.factory = base::BindRepeating(
        &CreatePrefService,
        base::BindRepeating([]() -> prefs::InProcessPrefServiceFactory* {
          return g_browser_process ? g_browser_process->pref_service_factory()
                                   : nullptr;
        }));
    info.task_runner = base::ThreadTaskRunnerHandle::Get();
    services->insert(
        std::make_pair(prefs::mojom::kLocalStateServiceName, info));
  }

#if defined(OS_ANDROID)
  {
    service_manager::EmbeddedServiceInfo info;
    info.factory =
        base::Bind(&proxy_resolver::ProxyResolverService::CreateService);
    services->insert(
        std::make_pair(proxy_resolver::mojom::kProxyResolverServiceName, info));
  }

  {
    service_manager::EmbeddedServiceInfo info;
    info.factory = base::BindRepeating(&StartDownloadManager);
    services->emplace("download_manager", info);
  }
#endif

#if defined(OS_CHROMEOS)
  if (base::FeatureList::IsEnabled(chromeos::features::kMultiDeviceApi)) {
    service_manager::EmbeddedServiceInfo info;
    info.factory = base::Bind(
        &chromeos::secure_channel::SecureChannelService::CreateService);
    info.task_runner = base::ThreadTaskRunnerHandle::Get();
    services->emplace(chromeos::secure_channel::mojom::kServiceName, info);
  }
#endif

#if defined(OS_CHROMEOS)
  ash_service_registry::RegisterInProcessServices(services, connection);
#endif

#if BUILDFLAG(ENABLE_SIMPLE_BROWSER_SERVICE_IN_PROCESS)
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kLaunchInProcessSimpleBrowserSwitch)) {
    service_manager::EmbeddedServiceInfo info;
    info.factory =
        base::BindRepeating([]() -> std::unique_ptr<service_manager::Service> {
          return std::make_unique<simple_browser::SimpleBrowserService>(
              simple_browser::SimpleBrowserService::UIInitializationMode::
                  kUseEnvironmentUI);
        });
    info.task_runner = base::SequencedTaskRunnerHandle::Get();
    services->emplace(simple_browser::mojom::kServiceName, std::move(info));
  }
#endif
}

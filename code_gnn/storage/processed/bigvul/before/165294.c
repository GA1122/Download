void ChromeContentBrowserClient::ExposeInterfacesToRenderer(
    service_manager::BinderRegistry* registry,
    blink::AssociatedInterfaceRegistry* associated_registry,
    content::RenderProcessHost* render_process_host) {
  associated_registry->AddInterface(
      base::Bind(&CacheStatsRecorder::Create, render_process_host->GetID()));

  scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner =
      base::CreateSingleThreadTaskRunnerWithTraits(
          {content::BrowserThread::UI});
  registry->AddInterface(
      base::Bind(&rappor::RapporRecorderImpl::Create,
                 g_browser_process->rappor_service()),
      ui_task_runner);
  registry->AddInterface(
      base::BindRepeating(&metrics::CallStackProfileCollector::Create));

  if (NetBenchmarking::CheckBenchmarkingEnabled()) {
    Profile* profile =
        Profile::FromBrowserContext(render_process_host->GetBrowserContext());
    auto* loading_predictor =
        predictors::LoadingPredictorFactory::GetForProfile(profile);
    registry->AddInterface(
        base::BindRepeating(
            &NetBenchmarking::Create,
            loading_predictor ? loading_predictor->GetWeakPtr() : nullptr,
            render_process_host->GetID()),
        ui_task_runner);
  }

#if defined(SAFE_BROWSING_DB_LOCAL) || defined(SAFE_BROWSING_DB_REMOTE)
  if (safe_browsing_service_) {
    content::ResourceContext* resource_context =
        render_process_host->GetBrowserContext()->GetResourceContext();
    registry->AddInterface(
        base::Bind(
            &safe_browsing::MojoSafeBrowsingImpl::MaybeCreate,
            render_process_host->GetID(), resource_context,
            base::Bind(
                &ChromeContentBrowserClient::GetSafeBrowsingUrlCheckerDelegate,
                base::Unretained(this), resource_context)),
        base::CreateSingleThreadTaskRunnerWithTraits({BrowserThread::IO}));
  }
#endif   

  if (data_reduction_proxy::params::IsEnabledWithNetworkService()) {
    registry->AddInterface(base::BindRepeating(
        &AddDataReductionProxyBinding,
        render_process_host->GetBrowserContext()->GetResourceContext()));
  }

#if defined(OS_WIN)
  auto get_process = base::BindRepeating(
      [](content::RenderProcessHost* host) -> base::Process {
        return host->GetProcess().Duplicate();
      },
      base::Unretained(render_process_host));
  registry->AddInterface(
      base::BindRepeating(&ModuleEventSinkImpl::Create, std::move(get_process),
                          content::PROCESS_TYPE_RENDERER,
                          base::Unretained(ModuleDatabase::GetInstance())),
      ui_task_runner);
#endif
#if defined(OS_ANDROID)
  Profile* profile =
      Profile::FromBrowserContext(render_process_host->GetBrowserContext());
  registry->AddInterface(
      base::BindRepeating(&android::AvailableOfflineContentProvider::Create,
                          profile),
      base::CreateSingleThreadTaskRunnerWithTraits({BrowserThread::UI}));
#endif

  for (auto* ep : extra_parts_) {
    ep->ExposeInterfacesToRenderer(registry, associated_registry,
                                   render_process_host);
  }
}

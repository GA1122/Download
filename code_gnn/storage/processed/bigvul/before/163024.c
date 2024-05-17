void ProfilingProcessHost::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
  content::RenderProcessHost* host =
      content::Source<content::RenderProcessHost>(source).ptr();

  if (host == profiled_renderer_ &&
      (type == content::NOTIFICATION_RENDERER_PROCESS_TERMINATED ||
       type == content::NOTIFICATION_RENDERER_PROCESS_CLOSED)) {
    DCHECK_EQ(mode(), Mode::kRendererSampling);
    profiled_renderer_ = nullptr;
  }
  if (type == content::NOTIFICATION_RENDERER_PROCESS_CREATED &&
      ShouldProfileNewRenderer(host)) {
    if (mode() == Mode::kRendererSampling) {
      profiled_renderer_ = host;
    }
    ProfilingClientBinder client(host);

    content::BrowserThread::GetTaskRunnerForThread(content::BrowserThread::IO)
        ->PostTask(
            FROM_HERE,
            base::BindOnce(&ProfilingProcessHost::AddClientToProfilingService,
                           base::Unretained(this), client.take(),
                           base::GetProcId(host->GetHandle()),
                           profiling::mojom::ProcessType::RENDERER));
  }
}

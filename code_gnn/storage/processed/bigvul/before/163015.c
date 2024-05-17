void ProfilingProcessHost::BrowserChildProcessLaunchedAndConnected(
    const content::ChildProcessData& data) {
  DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));

  DCHECK_NE(data.process_type, content::ProcessType::PROCESS_TYPE_RENDERER);

  if (!ShouldProfileProcessType(data.process_type)) {
    return;
  }

  profiling::mojom::ProcessType process_type =
      (data.process_type == content::ProcessType::PROCESS_TYPE_GPU)
          ? profiling::mojom::ProcessType::GPU
          : profiling::mojom::ProcessType::OTHER;

  content::BrowserThread::GetTaskRunnerForThread(content::BrowserThread::IO)
      ->PostTask(
          FROM_HERE,
          base::BindOnce(&ProfilingProcessHost::StartProfilingNonRendererChild,
                         base::Unretained(this), data.id,
                         base::GetProcId(data.handle), process_type));
}

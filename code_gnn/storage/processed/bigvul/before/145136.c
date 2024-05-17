  explicit ConnectionFilterImpl(int gpu_process_id) {
    auto task_runner = BrowserThread::GetTaskRunnerForThread(BrowserThread::UI);
    registry_.AddInterface(base::Bind(&FieldTrialRecorder::Create),
                           task_runner);
#if defined(OS_ANDROID)
    registry_.AddInterface(
        base::Bind(&BindJavaInterface<media::mojom::AndroidOverlayProvider>),
        task_runner);
#endif

    registry_.AddInterface(
        base::BindRepeating(&CreateMemoryCoordinatorHandleForGpuProcess,
                            gpu_process_id),
        task_runner);
  }

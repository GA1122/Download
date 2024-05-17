BrowserMainLoop::GetResizeTaskRunner() {
#if defined(OS_MACOSX)
  scoped_refptr<base::SingleThreadTaskRunner> task_runner =
      ui::WindowResizeHelperMac::Get()->task_runner();
  return task_runner ? task_runner : base::ThreadTaskRunnerHandle::Get();
#else
  return base::ThreadTaskRunnerHandle::Get();
#endif
}

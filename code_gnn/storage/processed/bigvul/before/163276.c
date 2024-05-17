void BrowserMainLoop::MainMessageLoopRun() {
#if defined(OS_ANDROID)
  NOTREACHED();
#else
  DCHECK(base::MessageLoopForUI::IsCurrent());
  if (parameters_.ui_task) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(FROM_HERE,
                                                  *parameters_.ui_task);
  }

  base::RunLoop run_loop;
  run_loop.Run();
#endif
}

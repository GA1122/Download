  void WaitForLauncherThread() {
    base::RunLoop run_loop;
    content::GetProcessLauncherTaskRunner()->PostTaskAndReply(
        FROM_HERE, base::DoNothing(), run_loop.QuitWhenIdleClosure());
    run_loop.Run();
  }

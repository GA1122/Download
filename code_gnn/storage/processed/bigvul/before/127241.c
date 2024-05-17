void WorkerProcessLauncherTest::StartWorker() {
  launcher_.reset(new WorkerProcessLauncher(
      task_runner_, launcher_delegate_.Pass(), &ipc_delegate_));
}

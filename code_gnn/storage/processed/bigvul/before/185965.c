   void ProcessControlLaunchFailed() {
//   void ProcessControlLaunchFailed(base::OnceClosure on_done) {
      ADD_FAILURE();
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE, base::RunLoop::QuitCurrentWhenIdleClosureDeprecated());
//     std::move(on_done).Run();
    }
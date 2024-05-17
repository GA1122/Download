  void RegisterDumpProvider(
      MemoryDumpProvider* mdp,
      scoped_refptr<base::SingleThreadTaskRunner> task_runner) {
    RegisterDumpProvider(mdp, task_runner, MemoryDumpProvider::Options());
  }

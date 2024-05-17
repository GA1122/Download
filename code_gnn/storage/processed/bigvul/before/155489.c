  explicit TestOptimizationGuideService(
      scoped_refptr<base::SingleThreadTaskRunner> io_thread_task_runner)
      : optimization_guide::OptimizationGuideService(io_thread_task_runner) {}

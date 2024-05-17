GetDefaultTaskSchedulerInitParams() {
#if defined(OS_ANDROID)
  return std::make_unique<base::TaskScheduler::InitParams>(
      base::SchedulerWorkerPoolParams(
          base::RecommendedMaxNumberOfThreadsInPool(2, 8, 0.1, 0),
          base::TimeDelta::FromSeconds(30)),
      base::SchedulerWorkerPoolParams(
          base::RecommendedMaxNumberOfThreadsInPool(2, 8, 0.1, 0),
          base::TimeDelta::FromSeconds(30)),
      base::SchedulerWorkerPoolParams(
          base::RecommendedMaxNumberOfThreadsInPool(3, 8, 0.3, 0),
          base::TimeDelta::FromSeconds(30)),
      base::SchedulerWorkerPoolParams(
          base::RecommendedMaxNumberOfThreadsInPool(3, 8, 0.3, 0),
          base::TimeDelta::FromSeconds(60)));
#else
  return std::make_unique<base::TaskScheduler::InitParams>(
      base::SchedulerWorkerPoolParams(
          base::RecommendedMaxNumberOfThreadsInPool(3, 8, 0.1, 0),
          base::TimeDelta::FromSeconds(30)),
      base::SchedulerWorkerPoolParams(
          base::RecommendedMaxNumberOfThreadsInPool(3, 8, 0.1, 0),
          base::TimeDelta::FromSeconds(40)),
      base::SchedulerWorkerPoolParams(
          base::RecommendedMaxNumberOfThreadsInPool(8, 32, 0.3, 0),
          base::TimeDelta::FromSeconds(30)),
      base::SchedulerWorkerPoolParams(
          base::RecommendedMaxNumberOfThreadsInPool(8, 32, 0.3, 0),
          base::TimeDelta::FromSeconds(60))
#if defined(OS_WIN)
          ,
      base::TaskScheduler::InitParams::SharedWorkerPoolEnvironment::COM_MTA
#endif   
      );
#endif
}

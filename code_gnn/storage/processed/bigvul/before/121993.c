void AppListController::ScheduleWarmup() {
  const int kInitWindowDelay = 5;
  base::MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&AppListController::LoadProfileForWarmup,
                 weak_factory_.GetWeakPtr()),
      base::TimeDelta::FromSeconds(kInitWindowDelay));

  const int kSendUsageStatsDelay = 5;
  base::MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&AppListController::SendAppListStats),
      base::TimeDelta::FromSeconds(kSendUsageStatsDelay));
}

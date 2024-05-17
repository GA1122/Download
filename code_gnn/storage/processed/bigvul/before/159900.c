void TopSitesImpl::ScheduleUpdateTimer() {
  if (timer_.IsRunning())
    return;

  timer_.Start(FROM_HERE, kDelayForUpdates, this,
               &TopSitesImpl::StartQueryForMostVisited);
}

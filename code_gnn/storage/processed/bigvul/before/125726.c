RenderViewHostImpl::~RenderViewHostImpl() {
  FOR_EACH_OBSERVER(
      RenderViewHostObserver, observers_, RenderViewHostDestruction());

  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_VIEW_HOST_DELETED,
      Source<RenderViewHost>(this),
      NotificationService::NoDetails());

  ClearPowerSaveBlockers();

  GetDelegate()->RenderViewDeleted(this);

  CrossSiteRequestManager::GetInstance()->SetHasPendingCrossSiteRequest(
      GetProcess()->GetID(), GetRoutingID(), false);
}

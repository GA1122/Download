BrowserRenderProcessHost::BrowserRenderProcessHost(Profile* profile)
    : RenderProcessHost(profile),
      visible_widgets_(0),
      backgrounded_(true),
      ALLOW_THIS_IN_INITIALIZER_LIST(cached_dibs_cleaner_(
            base::TimeDelta::FromSeconds(5),
            this, &BrowserRenderProcessHost::ClearTransportDIBCache)),
      zygote_child_(false) {
  widget_helper_ = new RenderWidgetHelper();

  registrar_.Add(this, NotificationType::USER_SCRIPTS_UPDATED,
                 NotificationService::AllSources());
  visited_link_updater_.reset(new VisitedLinkUpdater());

  WebCacheManager::GetInstance()->Add(id());
  ChildProcessSecurityPolicy::GetInstance()->Add(id());

}

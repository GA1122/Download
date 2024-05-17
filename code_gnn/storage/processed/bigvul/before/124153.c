void RenderViewHostManager::Init(BrowserContext* browser_context,
                                 SiteInstance* site_instance,
                                 int routing_id) {
  if (!site_instance)
    site_instance = SiteInstance::Create(browser_context);
  render_view_host_ = static_cast<RenderViewHostImpl*>(
      RenderViewHostFactory::Create(
          site_instance, render_view_delegate_, render_widget_delegate_,
          routing_id, false, delegate_->
          GetControllerForRenderManager().GetSessionStorageNamespace(
              site_instance)));

  registrar_.Add(this, NOTIFICATION_RENDERER_PROCESS_CLOSING,
                 NotificationService::AllSources());
}

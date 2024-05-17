RenderProcessHost* RenderProcessHostImpl::GetDefaultSubframeProcessHost(
    BrowserContext* browser_context,
    SiteInstanceImpl* site_instance,
    bool is_for_guests_only) {
  DefaultSubframeProcessHostHolder* holder =
      static_cast<DefaultSubframeProcessHostHolder*>(
          browser_context->GetUserData(&kDefaultSubframeProcessHostHolderKey));
  if (!holder) {
    holder = new DefaultSubframeProcessHostHolder(browser_context);
    browser_context->SetUserData(kDefaultSubframeProcessHostHolderKey,
                                 base::WrapUnique(holder));
  }

  return holder->GetProcessHost(site_instance, is_for_guests_only);
}

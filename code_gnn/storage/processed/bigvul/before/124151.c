SiteInstance* RenderViewHostManager::GetSiteInstanceForEntry(
    const NavigationEntryImpl& entry,
    SiteInstance* curr_instance) {

  const GURL& dest_url = entry.GetURL();
  NavigationControllerImpl& controller =
      delegate_->GetControllerForRenderManager();
  BrowserContext* browser_context = controller.GetBrowserContext();

  if (entry.site_instance())
    return entry.site_instance();

  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kProcessPerSite) &&
      entry.GetTransitionType() == PAGE_TRANSITION_GENERATED)
    return curr_instance;

  SiteInstanceImpl* curr_site_instance =
      static_cast<SiteInstanceImpl*>(curr_instance);

  if (!curr_site_instance->HasSite()) {
    bool use_process_per_site =
        RenderProcessHostImpl::ShouldUseProcessPerSite(browser_context,
                                                       dest_url) &&
        RenderProcessHostImpl::GetProcessHostForSite(browser_context, dest_url);
    if (curr_site_instance->HasRelatedSiteInstance(dest_url) ||
        use_process_per_site) {
      return curr_site_instance->GetRelatedSiteInstance(dest_url);
    }

    if (curr_site_instance->HasWrongProcessForURL(dest_url))
      return curr_site_instance->GetRelatedSiteInstance(dest_url);

    if (entry.IsViewSourceMode())
      return SiteInstance::CreateForURL(browser_context, dest_url);

    if (WebUIControllerFactoryRegistry::GetInstance()->UseWebUIForURL(
            browser_context, dest_url)) {
        return SiteInstance::CreateForURL(browser_context, dest_url);
    }

    if (entry.restore_type() != NavigationEntryImpl::RESTORE_NONE)
      curr_site_instance->SetSite(dest_url);

    return curr_site_instance;
  }


  NavigationEntry* curr_entry = controller.GetLastCommittedEntry();
  if (interstitial_page_) {
    curr_entry = controller.GetEntryAtOffset(-1);
  }
  const GURL& current_url = (curr_entry) ? curr_entry->GetURL() :
      curr_instance->GetSiteURL();

  if (curr_entry &&
      curr_entry->IsViewSourceMode() != entry.IsViewSourceMode()) {
    return SiteInstance::CreateForURL(browser_context, dest_url);
  }

  if (SiteInstance::IsSameWebSite(browser_context, current_url, dest_url) &&
      !static_cast<SiteInstanceImpl*>(curr_instance)->HasWrongProcessForURL(
          dest_url)) {
    return curr_instance;
  } else if (ShouldSwapProcessesForNavigation(curr_entry, &entry)) {
    return SiteInstance::CreateForURL(browser_context, dest_url);
  } else {
    return curr_instance->GetRelatedSiteInstance(dest_url);
  }
}

RenderProcessHost* RenderProcessHostImpl::GetProcessHostForSiteInstance(
    BrowserContext* browser_context,
    SiteInstanceImpl* site_instance) {
  const GURL site_url = site_instance->GetSiteURL();
  SiteInstanceImpl::ProcessReusePolicy process_reuse_policy =
      site_instance->process_reuse_policy();
  bool is_for_guests_only = site_url.SchemeIs(kGuestScheme);
  RenderProcessHost* render_process_host = nullptr;

  bool is_unmatched_service_worker = site_instance->is_for_service_worker();

  switch (process_reuse_policy) {
    case SiteInstanceImpl::ProcessReusePolicy::PROCESS_PER_SITE:
      render_process_host = GetProcessHostForSite(browser_context, site_url);
      break;
    case SiteInstanceImpl::ProcessReusePolicy::USE_DEFAULT_SUBFRAME_PROCESS:
      DCHECK(SiteIsolationPolicy::IsTopDocumentIsolationEnabled());
      DCHECK(!site_instance->is_for_service_worker());
      render_process_host = GetDefaultSubframeProcessHost(
          browser_context, site_instance, is_for_guests_only);
      break;
    case SiteInstanceImpl::ProcessReusePolicy::REUSE_PENDING_OR_COMMITTED_SITE:
      render_process_host =
          FindReusableProcessHostForSite(browser_context, site_url);
      UMA_HISTOGRAM_BOOLEAN(
          "SiteIsolation.ReusePendingOrCommittedSite.CouldReuse",
          render_process_host != nullptr);
      if (render_process_host)
        is_unmatched_service_worker = false;
      break;
    default:
      break;
  }

  if (!render_process_host &&
      !(process_reuse_policy == SiteInstanceImpl::ProcessReusePolicy::DEFAULT &&
        site_instance->is_for_service_worker())) {
    render_process_host = UnmatchedServiceWorkerProcessTracker::MatchWithSite(
        browser_context, site_url);
  }

  if (!render_process_host &&
      ShouldTryToUseExistingProcessHost(browser_context, site_url)) {
    render_process_host = GetExistingProcessHost(browser_context, site_url);
  }

  if (render_process_host &&
      !RenderProcessHostImpl::IsSuitableHost(render_process_host,
                                             browser_context, site_url)) {
    ChildProcessSecurityPolicyImpl* policy =
        ChildProcessSecurityPolicyImpl::GetInstance();
    base::debug::SetCrashKeyString(bad_message::GetRequestedSiteURLKey(),
                                   site_url.spec());
    base::debug::SetCrashKeyString(
        bad_message::GetKilledProcessOriginLockKey(),
        policy->GetOriginLock(render_process_host->GetID()).spec());
    CHECK(false) << "Unsuitable process reused for site " << site_url;
  }

  if (!render_process_host) {
    render_process_host = CreateOrUseSpareRenderProcessHost(
        browser_context, nullptr, site_instance, is_for_guests_only);
  }

  if (is_unmatched_service_worker) {
    UnmatchedServiceWorkerProcessTracker::Register(
        browser_context, render_process_host, site_url);
  }

  CHECK(render_process_host->InSameStoragePartition(
      BrowserContext::GetStoragePartition(browser_context, site_instance,
                                          false  )));

  return render_process_host;
}

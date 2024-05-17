content::PreviewsState ChromeContentBrowserClient::DetermineCommittedPreviews(
    content::PreviewsState initial_state,
    content::NavigationHandle* navigation_handle,
    const net::HttpResponseHeaders* response_headers) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  if (navigation_handle->IsErrorPage() ||
      !navigation_handle->GetURL().SchemeIsHTTPOrHTTPS()) {
    return content::PREVIEWS_OFF;
  }

  if (!previews::HasEnabledPreviews(initial_state) ||
      !navigation_handle->IsInMainFrame() ||
      navigation_handle->IsSameDocument()) {
    return initial_state;
  }

  PreviewsUITabHelper* ui_tab_helper =
      PreviewsUITabHelper::FromWebContents(navigation_handle->GetWebContents());
  if (!ui_tab_helper)
    return content::PREVIEWS_OFF;

  previews::PreviewsUserData* previews_user_data =
      ui_tab_helper->GetPreviewsUserData(navigation_handle);
  if (!previews_user_data)
    return content::PREVIEWS_OFF;

  PreviewsService* previews_service =
      PreviewsServiceFactory::GetForProfile(Profile::FromBrowserContext(
          navigation_handle->GetWebContents()->GetBrowserContext()));

  if (!previews_service || !previews_service->previews_ui_service())
    return content::PREVIEWS_OFF;

#if BUILDFLAG(ENABLE_OFFLINE_PAGES)
  offline_pages::OfflinePageTabHelper* tab_helper =
      offline_pages::OfflinePageTabHelper::FromWebContents(
          navigation_handle->GetWebContents());

  previews_user_data->set_offline_preview_used(
      tab_helper && tab_helper->GetOfflinePreviewItem());
#endif   

  if (response_headers &&
      response_headers->HasHeaderValue("cache-control", "no-transform")) {
    previews_user_data->set_cache_control_no_transform_directive();
  }

  previews::PreviewsDeciderImpl* previews_decider_impl =
      previews_service->previews_ui_service()->previews_decider_impl();
  DCHECK(previews_decider_impl);

  std::unique_ptr<data_reduction_proxy::DataReductionProxyData> drp_data;
  auto* settings =
      DataReductionProxyChromeSettingsFactory::GetForBrowserContext(
          navigation_handle->GetWebContents()->GetBrowserContext());
  if (settings) {
    drp_data = settings->CreateDataFromNavigationHandle(navigation_handle,
                                                        response_headers);
  }

  content::PreviewsState committed_state = DetermineCommittedPreviewsForURL(
      navigation_handle->GetURL(), drp_data.get(), previews_user_data,
      previews_decider_impl, initial_state);

  DCHECK(!previews_user_data->cache_control_no_transform_directive() ||
         !previews::HasEnabledPreviews(committed_state));

  previews_user_data->set_committed_previews_state(committed_state);

  previews::PreviewsType committed_type =
      previews::GetMainFramePreviewsType(committed_state);

  previews_user_data->SetCommittedPreviewsType(committed_type);

  std::vector<previews::PreviewsEligibilityReason> passed_reasons;
  previews_decider_impl->LogPreviewDecisionMade(
      (previews_user_data->cache_control_no_transform_directive()
           ? previews::PreviewsEligibilityReason::CACHE_CONTROL_NO_TRANSFORM
           : previews::PreviewsEligibilityReason::COMMITTED),
      navigation_handle->GetURL(), base::Time::Now(),
      previews_user_data->committed_previews_type(), std::move(passed_reasons),
      previews_user_data->page_id());

  return committed_state;
}

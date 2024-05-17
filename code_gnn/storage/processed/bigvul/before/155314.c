ChromeContentBrowserClient::DetermineCommittedPreviewsForURL(
    const GURL& url,
    data_reduction_proxy::DataReductionProxyData* drp_data,
    previews::PreviewsUserData* previews_user_data,
    const previews::PreviewsDecider* previews_decider,
    content::PreviewsState initial_state,
    content::NavigationHandle* navigation_handle) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  if (!previews::HasEnabledPreviews(initial_state))
    return content::PREVIEWS_OFF;

  content::PreviewsState previews_state =
      data_reduction_proxy::ContentLoFiDecider::
          DetermineCommittedServerPreviewsState(drp_data, initial_state);

  return previews::DetermineCommittedClientPreviewsState(
      previews_user_data, url, previews_state, previews_decider,
      navigation_handle);
}

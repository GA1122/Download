void BackgroundLoaderOffliner::DidFinishNavigation(
    content::NavigationHandle* navigation_handle) {
  if (!navigation_handle->IsInMainFrame())
    return;
  if (navigation_handle->IsErrorPage()) {
    RecordErrorCauseUMA(pending_request_->client_id(),
                        static_cast<int>(navigation_handle->GetNetErrorCode()));
    page_load_state_ = RETRIABLE_NET_ERROR;
  } else {
    int status_code = 200;   
    if (navigation_handle->GetResponseHeaders())
      status_code = navigation_handle->GetResponseHeaders()->response_code();
    if (status_code == 301 || (status_code >= 400 && status_code != 418)) {
      RecordErrorCauseUMA(pending_request_->client_id(), status_code);
      page_load_state_ = RETRIABLE_HTTP_ERROR;
    }
  }

  PreviewsUITabHelper* previews_tab_helper =
      PreviewsUITabHelper::FromWebContents(navigation_handle->GetWebContents());
  content::PreviewsState previews_state = content::PREVIEWS_OFF;
  if (previews_tab_helper) {
    previews::PreviewsUserData* previews_user_data =
        previews_tab_helper->GetPreviewsUserData(navigation_handle);
    if (previews_user_data)
      previews_state = previews_user_data->CommittedPreviewsState();
  }

  RecordOffliningPreviewsUMA(pending_request_->client_id(), previews_state);
}

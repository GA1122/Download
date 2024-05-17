void SavePageIfNotNavigatedAway(const GURL& url,
                                const GURL& original_url,
                                const ScopedJavaGlobalRef<jobject>& j_tab_ref,
                                const std::string& origin) {
  content::WebContents* web_contents = GetWebContentsFromJavaTab(j_tab_ref);
  if (!web_contents)
    return;

  GURL current_url = web_contents->GetLastCommittedURL();
  if (!OfflinePageUtils::EqualsIgnoringFragment(current_url, url))
    return;

  offline_pages::ClientId client_id;
  client_id.name_space = offline_pages::kDownloadNamespace;
  client_id.id = base::GenerateGUID();
  int64_t request_id = OfflinePageModel::kInvalidOfflineId;

  if (offline_pages::IsBackgroundLoaderForDownloadsEnabled()) {
    offline_pages::RequestCoordinator* request_coordinator =
        offline_pages::RequestCoordinatorFactory::GetForBrowserContext(
            web_contents->GetBrowserContext());
    if (request_coordinator) {
      offline_pages::RequestCoordinator::SavePageLaterParams params;
      params.url = current_url;
      params.client_id = client_id;
      params.availability =
          RequestCoordinator::RequestAvailability::DISABLED_FOR_OFFLINER;
      params.original_url = original_url;
      params.request_origin = origin;
      request_id =
          request_coordinator->SavePageLater(params, base::DoNothing());
    } else {
      DVLOG(1) << "SavePageIfNotNavigatedAway has no valid coordinator.";
    }
  }

  offline_pages::RecentTabHelper* tab_helper =
      RecentTabHelper::FromWebContents(web_contents);
  if (!tab_helper) {
    if (request_id != OfflinePageModel::kInvalidOfflineId) {
      offline_pages::RequestCoordinator* request_coordinator =
          offline_pages::RequestCoordinatorFactory::GetForBrowserContext(
              web_contents->GetBrowserContext());
      if (request_coordinator)
        request_coordinator->EnableForOffliner(request_id, client_id);
      else
        DVLOG(1) << "SavePageIfNotNavigatedAway has no valid coordinator.";
    }
    return;
  }
  tab_helper->ObserveAndDownloadCurrentPage(client_id, request_id, origin);

  OfflinePageNotificationBridge notification_bridge;
  notification_bridge.ShowDownloadingToast();
}

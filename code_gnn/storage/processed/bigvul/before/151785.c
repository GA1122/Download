void BackgroundLoaderOffliner::DeleteOfflinePageCallback(
    const SavePageRequest& request,
    DeletePageResult result) {
  std::move(cancel_callback_).Run(request);
}

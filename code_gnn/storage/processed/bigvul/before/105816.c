void UrlmonUrlRequestManager::BindTerminated(IMoniker* moniker,
                                             IBindCtx* bind_ctx,
                                             IStream* post_data,
                                             const char* request_headers) {
  DownloadInHostParams* download_params = new DownloadInHostParams;
  download_params->bind_ctx = bind_ctx;
  download_params->moniker = moniker;
  download_params->post_data = post_data;
  if (request_headers) {
    download_params->request_headers = request_headers;
  }
  ::PostMessage(notification_window_, WM_DOWNLOAD_IN_HOST,
        reinterpret_cast<WPARAM>(download_params), 0);
}

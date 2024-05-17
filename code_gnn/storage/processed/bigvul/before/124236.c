net::Error CallbackAndReturn(
    const DownloadResourceHandler::OnStartedCallback& started_cb,
    net::Error net_error) {
  if (started_cb.is_null())
    return net_error;
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(started_cb, static_cast<DownloadItem*>(NULL), net_error));

  return net_error;
}

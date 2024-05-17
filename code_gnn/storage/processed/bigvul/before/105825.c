HRESULT UrlmonUrlRequest::InitPending(const GURL& url, IMoniker* moniker,
                                      IBindCtx* bind_context,
                                      bool enable_frame_busting,
                                      bool privileged_mode,
                                      HWND notification_window,
                                      IStream* cache) {
  DVLOG(1) << __FUNCTION__ << me() << url.spec();
  DCHECK(bind_context_ == NULL);
  DCHECK(moniker_ == NULL);
  DCHECK(cache_ == NULL);
  DCHECK(thread_ == 0 || thread_ == base::PlatformThread::CurrentId());
  thread_ = base::PlatformThread::CurrentId();
  bind_context_ = bind_context;
  moniker_ = moniker;
  enable_frame_busting_ = enable_frame_busting;
  privileged_mode_ = privileged_mode;
  parent_window_ = notification_window;
  cache_ = cache;
  set_url(url.spec());
  set_pending(true);

  method_ = "get";
  return S_OK;
}

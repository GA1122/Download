void UrlmonUrlRequestManager::StartRequest(int request_id,
    const AutomationURLRequest& request_info) {
  DVLOG(1) << __FUNCTION__ << " id: " << request_id;
  DCHECK_EQ(0, calling_delegate_);

  if (stopping_) {
    DLOG(WARNING) << __FUNCTION__ << " request not started (stopping)";
    return;
  }

  DCHECK(request_map_.find(request_id) == request_map_.end());
  DCHECK(GURL(request_info.url).is_valid());

  scoped_refptr<UrlmonUrlRequest> new_request;
  bool is_started = false;
  if (pending_request_) {
    if (pending_request_->url() != request_info.url) {
      DLOG(INFO) << __FUNCTION__
                 << "Received url request for url:"
                 << request_info.url
                 << ". Stopping pending url request for url:"
                 << pending_request_->url();
      pending_request_->Stop();
      pending_request_ = NULL;
    } else {
      new_request.swap(pending_request_);
      is_started = true;
      DVLOG(1) << __FUNCTION__ << new_request->me()
               << " assigned id " << request_id;
    }
  }

  if (!is_started) {
    CComObject<UrlmonUrlRequest>* created_request = NULL;
    CComObject<UrlmonUrlRequest>::CreateInstance(&created_request);
    new_request = created_request;
  }

  new_request->Initialize(static_cast<PluginUrlRequestDelegate*>(this),
      request_id,
      request_info.url,
      request_info.method,
      request_info.referrer,
      request_info.extra_request_headers,
      request_info.upload_data,
      static_cast<ResourceType::Type>(request_info.resource_type),
      enable_frame_busting_,
      request_info.load_flags);
  new_request->set_parent_window(notification_window_);
  new_request->set_privileged_mode(privileged_mode_);

  request_map_[request_id] = new_request;

  if (!is_started) {
    new_request->Start();
  } else {
    DCHECK(!new_request->response_headers().empty());
    new_request->OnResponse(
        0, UTF8ToWide(new_request->response_headers()).c_str(), NULL, NULL);
  }
}

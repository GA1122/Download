void ResourceDispatcherHost::BeginDownload(
    const GURL& url,
    const GURL& referrer,
    const DownloadSaveInfo& save_info,
    int child_id,
    int route_id,
    URLRequestContext* request_context) {
  if (is_shutdown_)
    return;

  if (!ChildProcessSecurityPolicy::GetInstance()->
          CanRequestURL(child_id, url)) {
    LOG(INFO) << "Denied unauthorized download request for " <<
        url.possibly_invalid_spec();
    return;
  }

  PluginService::GetInstance()->LoadChromePlugins(this);
  URLRequest* request = new URLRequest(url, this);

  request_id_--;

  scoped_refptr<ResourceHandler> handler =
      new DownloadResourceHandler(this,
                                  child_id,
                                  route_id,
                                  request_id_,
                                  url,
                                  download_file_manager_.get(),
                                  request,
                                  true,
                                  save_info);


  if (safe_browsing_->enabled() && safe_browsing_->CanCheckUrl(url)) {
    handler = new SafeBrowsingResourceHandler(handler,
                                              child_id,
                                              route_id,
                                              ResourceType::MAIN_FRAME,
                                              safe_browsing_,
                                              this,
                                              receiver_);
  }

  if (!URLRequest::IsHandledURL(url)) {
    LOG(INFO) << "Download request for unsupported protocol: " <<
        url.possibly_invalid_spec();
    return;
  }

  request->set_method("GET");
  request->set_referrer(CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kNoReferrers) ? std::string() : referrer.spec());
  request->set_context(request_context);
  request->set_load_flags(request->load_flags() |
      net::LOAD_IS_DOWNLOAD);

  ResourceDispatcherHostRequestInfo* extra_info =
      new ResourceDispatcherHostRequestInfo(handler,
                                            ChildProcessInfo::RENDER_PROCESS,
                                            child_id,
                                            route_id,
                                            request_id_,
                                            "null",   
                                            "null",   
                                            ResourceType::SUB_RESOURCE,
                                            0,   
                                            true,   
                                            true,  
                                            -1,  
                                            -1);  
  SetRequestInfo(request, extra_info);   
  chrome_browser_net::SetOriginProcessUniqueIDForRequest(child_id, request);

  BeginRequestInternal(request);
}

 int32_t PPB_URLLoader_Impl::Open(PP_Resource request_id,
                                 PP_CompletionCallback callback) {
  EnterResourceNoLock<PPB_URLRequestInfo_API> enter_request(request_id, true);
  if (enter_request.failed())
    return PP_ERROR_BADARGUMENT;
  PPB_URLRequestInfo_Impl* request = static_cast<PPB_URLRequestInfo_Impl*>(
      enter_request.object());

  int32_t rv = ValidateCallback(callback);
  if (rv != PP_OK)
    return rv;

  if (request->RequiresUniversalAccess() && !has_universal_access_)
    return PP_ERROR_NOACCESS;

  if (loader_.get())
    return PP_ERROR_INPROGRESS;

  WebFrame* frame = instance()->container()->element().document().frame();
  if (!frame)
    return PP_ERROR_FAILED;
  WebURLRequest web_request(request->ToWebURLRequest(frame));

  WebURLLoaderOptions options;
  if (has_universal_access_) {
    options.crossOriginRequestPolicy =
        WebURLLoaderOptions::CrossOriginRequestPolicyAllow;
    options.allowCredentials = true;
  } else if (request->allow_cross_origin_requests()) {
    options.crossOriginRequestPolicy =
        WebURLLoaderOptions::CrossOriginRequestPolicyUseAccessControl;
    options.allowCredentials = request->allow_credentials();
  }

  is_asynchronous_load_suspended_ = false;
  loader_.reset(frame->createAssociatedURLLoader(options));
  if (!loader_.get())
    return PP_ERROR_FAILED;

  loader_->loadAsynchronously(web_request, this);
  if (done_status_ == PP_ERROR_FAILED ||
      done_status_ == PP_ERROR_NOACCESS)
    return done_status_;

  request_info_ = scoped_refptr<PPB_URLRequestInfo_Impl>(request);

  RegisterCallback(callback);
  return PP_OK_COMPLETIONPENDING;
}

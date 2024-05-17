int32_t PPB_URLLoader_Impl::FollowRedirect(PP_CompletionCallback callback) {
  int32_t rv = ValidateCallback(callback);
  if (rv != PP_OK)
    return rv;

  WebURL redirect_url = GURL(response_info_->redirect_url());

  loader_->setDefersLoading(false);   
  RegisterCallback(callback);
  return PP_OK_COMPLETIONPENDING;
}

int32_t PPB_URLLoader_Impl::FollowRedirect(
    scoped_refptr<TrackedCallback> callback) {
  int32_t rv = ValidateCallback(callback);
  if (rv != PP_OK)
    return rv;

  WebURL redirect_url = GURL(response_info_->redirect_url());

  SetDefersLoading(false);   
  RegisterCallback(callback);
  return PP_OK_COMPLETIONPENDING;
}

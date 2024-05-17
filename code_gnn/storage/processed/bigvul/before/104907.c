void PPB_URLLoader_Impl::willSendRequest(
    WebURLLoader* loader,
    WebURLRequest& new_request,
    const WebURLResponse& redirect_response) {
  if (!request_info_->follow_redirects()) {
    SaveResponse(redirect_response);
    loader_->setDefersLoading(true);
    RunCallback(PP_OK);
  }
}

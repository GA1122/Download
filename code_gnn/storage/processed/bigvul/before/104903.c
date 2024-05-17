void PPB_URLLoader_Impl::didFinishLoading(WebURLLoader* loader,
                                          double finish_time) {
  done_status_ = PP_OK;
  RunCallback(done_status_);
}

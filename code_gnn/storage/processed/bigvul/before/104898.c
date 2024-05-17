void PPB_URLLoader_Impl::SetStatusCallback(
    PP_URLLoaderTrusted_StatusCallback cb) {
  status_callback_ = cb;
}

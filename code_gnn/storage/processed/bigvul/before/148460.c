void WebContentsImpl::OnAllPasswordInputsHiddenOnHttp() {
  controller_.ssl_manager()->DidHideAllPasswordInputsOnHttp();
}

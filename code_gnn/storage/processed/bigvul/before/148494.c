void WebContentsImpl::OnPasswordInputShownOnHttp() {
  controller_.ssl_manager()->DidShowPasswordInputOnHttp();
}

void WebContentsImpl::OnCreditCardInputShownOnHttp() {
  controller_.ssl_manager()->DidShowCreditCardInputOnHttp();
}

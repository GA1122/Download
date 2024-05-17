bool RenderFrameImpl::OverrideLegacySymantecCertConsoleMessage(
    const blink::WebURL& url,
    blink::WebString* console_message) {
  std::string console_message_string;
  if (GetContentClient()->renderer()->OverrideLegacySymantecCertConsoleMessage(
          GURL(url), &console_message_string)) {
    *console_message = blink::WebString::FromASCII(console_message_string);
    return true;
  }
  return false;
}

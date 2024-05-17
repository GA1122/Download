void WebFrameLoaderClient::didRunInsecureContent(SecurityOrigin* origin) {
  if (webframe_->client()) {
    webframe_->client()->didRunInsecureContent(webframe_,
        webkit_glue::SecurityOriginToWebSecurityOrigin(origin));
  }
}

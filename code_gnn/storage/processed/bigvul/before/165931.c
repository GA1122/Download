void RenderFrameImpl::ReportLegacySymantecCert(const blink::WebURL& url,
                                               bool did_fail) {
  url::Origin origin = url::Origin::Create(GURL(url));
  if (base::ContainsKey(certificate_warning_origins_, origin))
    return;

  size_t num_warnings = certificate_warning_origins_.size();
  if (num_warnings > kMaxSecurityWarningMessages)
    return;

  std::string console_message;

  if (num_warnings == kMaxSecurityWarningMessages) {
    if (did_fail) {
      console_message =
          "Additional resources on this page were loaded with "
          "SSL certificates that have been "
          "distrusted. See "
          "https://g.co/chrome/symantecpkicerts for "
          "more information.";
    } else {
      console_message =
          "Additional resources on this page were loaded with "
          "SSL certificates that will be "
          "distrusted in the future. "
          "Once distrusted, users will be prevented from "
          "loading these resources. See "
          "https://g.co/chrome/symantecpkicerts for "
          "more information.";
    }
  } else {
    if (did_fail) {
      console_message = base::StringPrintf(
          "The SSL certificate used to load resources from %s"
          " has been distrusted. See "
          "https://g.co/chrome/symantecpkicerts for "
          "more information.",
          origin.Serialize().c_str());
    } else if (!GetContentClient()
                    ->renderer()
                    ->OverrideLegacySymantecCertConsoleMessage(
                        GURL(url), &console_message)) {
      console_message = base::StringPrintf(
          "The SSL certificate used to load resources from %s"
          " will be "
          "distrusted in the future. "
          "Once distrusted, users will be prevented from "
          "loading these resources. See "
          "https://g.co/chrome/symantecpkicerts for "
          "more information.",
          origin.Serialize().c_str());
    }
  }
  certificate_warning_origins_.insert(origin);
  AddMessageToConsole((frame_->Parent() && !did_fail)
                          ? CONSOLE_MESSAGE_LEVEL_VERBOSE
                          : CONSOLE_MESSAGE_LEVEL_WARNING,
                      console_message);
}

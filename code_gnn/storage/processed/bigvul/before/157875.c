void WebContentsImpl::ShowInsecureLocalhostWarningIfNeeded() {
  bool allow_localhost = base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kAllowInsecureLocalhost);
  if (!allow_localhost)
    return;

  content::NavigationEntry* entry = GetController().GetLastCommittedEntry();
  if (!entry || !net::IsLocalhost(entry->GetURL()))
    return;

  content::SSLStatus ssl_status = entry->GetSSL();
  bool is_cert_error = net::IsCertStatusError(ssl_status.cert_status) &&
                       !net::IsCertStatusMinorError(ssl_status.cert_status);
  if (!is_cert_error)
    return;

  GetMainFrame()->AddMessageToConsole(
      content::CONSOLE_MESSAGE_LEVEL_WARNING,
      base::StringPrintf("This site does not have a valid SSL "
                         "certificate! Without SSL, your site's and "
                         "visitors' data is vulnerable to theft and "
                         "tampering. Get a valid SSL certificate before"
                         " releasing your website to the public."));
}

void ExternalProtocolHandler::PermitLaunchUrl() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  g_accept_requests = true;
}

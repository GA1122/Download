bool NotifyCertificateError(
    DevToolsAgentHost* host,
    int cert_error,
    const GURL& request_url,
    const DevToolsAgentHostImpl::CertErrorCallback& callback) {
  DevToolsAgentHostImpl* host_impl = static_cast<DevToolsAgentHostImpl*>(host);
  for (auto* security_handler :
       protocol::SecurityHandler::ForAgentHost(host_impl)) {
    if (security_handler->NotifyCertificateError(cert_error, request_url,
                                                 callback)) {
      return true;
    }
  }
  return false;
}

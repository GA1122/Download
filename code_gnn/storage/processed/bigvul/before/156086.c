bool IsYoutubeDomainUrl(const GURL& url,
                        SubdomainPermission subdomain_permission,
                        PortPermission port_permission) {
  return IsValidURL(url, port_permission) &&
         IsValidHostName(url.host_piece(), "youtube", subdomain_permission,
                         nullptr);
}

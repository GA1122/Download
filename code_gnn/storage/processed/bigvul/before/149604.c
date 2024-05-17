PreresolveJob::PreresolveJob(const GURL& url,
                             int num_sockets,
                             bool allow_credentials,
                             net::NetworkIsolationKey network_isolation_key,
                             PreresolveInfo* info)
    : url(url),
      num_sockets(num_sockets),
      allow_credentials(allow_credentials),
      network_isolation_key(std::move(network_isolation_key)),
      info(info) {
  DCHECK_GE(num_sockets, 0);
}

int ProxyClientSocket::HandleProxyAuthChallenge(HttpAuthController* auth,
                                                HttpResponseInfo* response,
                                                const BoundNetLog& net_log) {
  DCHECK(response->headers.get());
  int rv = auth->HandleAuthChallenge(response->headers, false, true, net_log);
  response->auth_challenge = auth->auth_info();
  if (rv == OK)
    return ERR_PROXY_AUTH_REQUESTED;
  return rv;
}

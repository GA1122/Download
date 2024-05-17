std::string GetSocketSessionCacheKey(const SSLClientSocketOpenSSL& socket) {
  std::string result = socket.host_and_port().ToString();
  result.append("/");
  result.append(socket.ssl_session_cache_shard());
  return result;
}

void ProxyClientSocket::LogBlockedTunnelResponse(int http_status_code,
                                                 const GURL& url,
                                                 bool is_https_proxy) {
  if (is_https_proxy) {
    UMA_HISTOGRAM_CUSTOM_ENUMERATION(
        "Net.BlockedTunnelResponse.HttpsProxy",
        HttpUtil::MapStatusCodeForHistogram(http_status_code),
        HttpUtil::GetStatusCodesForHistogram());
  } else {
    UMA_HISTOGRAM_CUSTOM_ENUMERATION(
        "Net.BlockedTunnelResponse.HttpProxy",
        HttpUtil::MapStatusCodeForHistogram(http_status_code),
        HttpUtil::GetStatusCodesForHistogram());
  }
 }

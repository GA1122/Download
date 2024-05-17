bool HasServerLoFiResponseHeaders(const ResourceResponse& response) {
  return response.HttpHeaderField("chrome-proxy-content-transform")
             .Contains("empty-image") ||
         response.HttpHeaderField("chrome-proxy").Contains("q=low");
}

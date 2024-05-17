static bool IsDownloadOrStreamRequest(const ResourceRequest& request) {
  return request.DownloadToFile() || request.UseStreamOnResponse();
}

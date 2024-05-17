void CheckClientDownloadRequest::MaybeStorePingsForDownload(
    DownloadCheckResult result,
    bool upload_requested,
    const std::string& request_data,
    const std::string& response_body) {
  DownloadFeedbackService::MaybeStorePingsForDownload(
      result, upload_requested, item_, request_data, response_body);
}

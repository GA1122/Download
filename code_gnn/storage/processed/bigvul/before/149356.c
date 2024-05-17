void BinaryUploadService::OnUploadComplete(Request* request,
                                           bool success,
                                           const std::string& response_data) {
  if (!IsActive(request))
    return;

  if (!success) {
    FinishRequest(request, Result::UPLOAD_FAILURE,
                  DeepScanningClientResponse());
    return;
  }

  DeepScanningClientResponse response;
  if (!response.ParseFromString(response_data)) {
    FinishRequest(request, Result::UPLOAD_FAILURE,
                  DeepScanningClientResponse());
    return;
  }

  active_uploads_.erase(request);

  OnGetResponse(request, response);
}

void BinaryUploadService::OnGetInstanceID(Request* request,
                                          const std::string& instance_id) {
  if (!IsActive(request))
    return;

  if (instance_id == BinaryFCMService::kInvalidId) {
    FinishRequest(request, Result::FAILED_TO_GET_TOKEN,
                  DeepScanningClientResponse());
    return;
  }

  request->set_fcm_token(instance_id);
  request->GetRequestData(base::BindOnce(&BinaryUploadService::OnGetRequestData,
                                         weakptr_factory_.GetWeakPtr(),
                                         request));
}

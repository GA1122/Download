inline void ValidateDataUploadRequest::GetRequestData(DataCallback callback) {
  std::move(callback).Run(BinaryUploadService::Result::SUCCESS,
                          BinaryUploadService::Request::Data());
}

status_t Camera3Device::capture(CameraMetadata &request, int64_t*  ) {
    ATRACE_CALL();

 List<const CameraMetadata> requests;
    requests.push_back(request);
 return captureList(requests,  NULL);
}

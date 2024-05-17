status_t Camera3Device::setStreamingRequest(const CameraMetadata &request,
 int64_t*  ) {
    ATRACE_CALL();

 List<const CameraMetadata> requests;
    requests.push_back(request);
 return setStreamingRequestList(requests,  NULL);
}

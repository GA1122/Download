status_t Camera3Device::setStreamingRequestList(const List<const CameraMetadata> &requests,
 int64_t *lastFrameNumber) {
    ATRACE_CALL();

 return submitRequestsHelper(requests,  true, lastFrameNumber);
}

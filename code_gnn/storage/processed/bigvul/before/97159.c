ResourceError WebFrameLoaderClient::cancelledError(
    const ResourceRequest& request) {
  return ResourceError(net::kErrorDomain, net::ERR_ABORTED,
                       request.url().string(), String());
}

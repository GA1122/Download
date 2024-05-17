bool WebFrameLoaderClient::shouldFallBack(const ResourceError& error) {
  return error.errorCode() != net::ERR_ABORTED;
}

ResourceError WebFrameLoaderClient::interruptForPolicyChangeError(
    const ResourceRequest& request) {
  return ResourceError(kInternalErrorDomain, ERR_POLICY_CHANGE,
                       request.url().string(), String());
}

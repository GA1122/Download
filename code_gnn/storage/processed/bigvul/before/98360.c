bool FrameLoaderClient::shouldFallBack(const ResourceError& error)
{
    return !(error.isCancellation() || error.errorCode() == WEBKIT_POLICY_ERROR_FRAME_LOAD_INTERRUPTED_BY_POLICY_CHANGE);
}

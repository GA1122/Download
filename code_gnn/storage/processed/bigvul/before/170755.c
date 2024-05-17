void OMXNodeInstance::setHandle(OMX::node_id node_id, OMX_HANDLETYPE handle) {
    mNodeID = node_id;
    CLOG_LIFE(allocateNode, "handle=%p", handle);
    CHECK(mHandle == NULL);
    mHandle = handle;
}

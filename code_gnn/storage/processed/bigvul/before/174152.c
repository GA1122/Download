status_t OMX::getParameter(
        node_id node, OMX_INDEXTYPE index,
 void *params, size_t size) {
    ALOGV("getParameter(%u %#x %p %zd)", node, index, params, size);
 return findInstance(node)->getParameter(
            index, params, size);
}

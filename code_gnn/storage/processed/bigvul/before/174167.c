status_t OMX::setParameter(
        node_id node, OMX_INDEXTYPE index,
 const void *params, size_t size) {
    ALOGV("setParameter(%u %#x %p %zd)", node, index, params, size);
 return findInstance(node)->setParameter(
            index, params, size);
}

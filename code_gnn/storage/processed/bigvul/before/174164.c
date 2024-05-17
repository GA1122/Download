status_t OMX::setConfig(
        node_id node, OMX_INDEXTYPE index,
 const void *params, size_t size) {
 return findInstance(node)->setConfig(
            index, params, size);
}

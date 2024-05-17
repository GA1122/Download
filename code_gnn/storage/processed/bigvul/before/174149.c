status_t OMX::getConfig(
        node_id node, OMX_INDEXTYPE index,
 void *params, size_t size) {
 return findInstance(node)->getConfig(
            index, params, size);
}

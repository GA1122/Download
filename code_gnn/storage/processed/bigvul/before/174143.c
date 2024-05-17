status_t OMX::enableGraphicBuffers(
        node_id node, OMX_U32 port_index, OMX_BOOL enable) {
 return findInstance(node)->enableGraphicBuffers(port_index, enable);
}

status_t OMX::freeBuffer(node_id node, OMX_U32 port_index, buffer_id buffer) {
 return findInstance(node)->freeBuffer(
            port_index, buffer);
}

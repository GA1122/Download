status_t OMX::allocateBuffer(
        node_id node, OMX_U32 port_index, size_t size,
        buffer_id *buffer, void **buffer_data) {
 return findInstance(node)->allocateBuffer(
            port_index, size, buffer, buffer_data);
}

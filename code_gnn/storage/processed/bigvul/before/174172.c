status_t OMX::useBuffer(
        node_id node, OMX_U32 port_index, const sp<IMemory> &params,
        buffer_id *buffer, OMX_U32 allottedSize) {
 return findInstance(node)->useBuffer(
            port_index, params, buffer, allottedSize);
}
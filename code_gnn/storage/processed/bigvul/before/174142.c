status_t OMX::emptyBuffer(
        node_id node,
        buffer_id buffer,
        OMX_U32 range_offset, OMX_U32 range_length,
        OMX_U32 flags, OMX_TICKS timestamp, int fenceFd) {
 return findInstance(node)->emptyBuffer(
            buffer, range_offset, range_length, flags, timestamp, fenceFd);
}

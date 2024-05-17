status_t OMX::useGraphicBuffer(
        node_id node, OMX_U32 port_index,
 const sp<GraphicBuffer> &graphicBuffer, buffer_id *buffer) {
 return findInstance(node)->useGraphicBuffer(
            port_index, graphicBuffer, buffer);
}

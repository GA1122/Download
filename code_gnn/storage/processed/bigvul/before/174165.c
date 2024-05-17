status_t OMX::setInputSurface(
        node_id node, OMX_U32 port_index,
 const sp<IGraphicBufferConsumer> &bufferConsumer, MetadataBufferType *type) {
 return findInstance(node)->setInputSurface(port_index, bufferConsumer, type);
}

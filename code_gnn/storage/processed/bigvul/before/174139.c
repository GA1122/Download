status_t OMX::createInputSurface(
        node_id node, OMX_U32 port_index,
        sp<IGraphicBufferProducer> *bufferProducer, MetadataBufferType *type) {
 return findInstance(node)->createInputSurface(
            port_index, bufferProducer, type);
}

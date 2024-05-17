status_t OMX::storeMetaDataInBuffers(
        node_id node, OMX_U32 port_index, OMX_BOOL enable, MetadataBufferType *type) {
 return findInstance(node)->storeMetaDataInBuffers(port_index, enable, type);
}

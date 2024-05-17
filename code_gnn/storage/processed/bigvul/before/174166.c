status_t OMX::setInternalOption(
        node_id node,
        OMX_U32 port_index,
 InternalOptionType type,
 const void *data,
 size_t size) {
 return findInstance(node)->setInternalOption(port_index, type, data, size);
}

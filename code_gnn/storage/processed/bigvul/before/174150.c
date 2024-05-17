status_t OMX::getExtensionIndex(
        node_id node,
 const char *parameter_name,
        OMX_INDEXTYPE *index) {
 return findInstance(node)->getExtensionIndex(
            parameter_name, index);
}

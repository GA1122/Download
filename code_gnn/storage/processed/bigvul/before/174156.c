bool OMX::isSecure(node_id node) {
 OMXNodeInstance *instance = findInstance(node);
 return (instance == NULL ? false : instance->isSecure());
}

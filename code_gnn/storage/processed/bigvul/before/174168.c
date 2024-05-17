status_t OMX::signalEndOfInputStream(node_id node) {
 return findInstance(node)->signalEndOfInputStream();
}

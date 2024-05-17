status_t OMX::getState(
        node_id node, OMX_STATETYPE* state) {
 return findInstance(node)->getState(
            state);
}

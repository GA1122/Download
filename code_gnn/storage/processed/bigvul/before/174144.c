status_t OMX::fillBuffer(node_id node, buffer_id buffer, int fenceFd) {
 return findInstance(node)->fillBuffer(buffer, fenceFd);
}

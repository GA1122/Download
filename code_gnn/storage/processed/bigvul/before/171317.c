size_t OMXCodec::countBuffersWeOwn(const Vector<BufferInfo> &buffers) {
 size_t n = 0;
 for (size_t i = 0; i < buffers.size(); ++i) {
 if (buffers[i].mStatus != OWNED_BY_COMPONENT) {
 ++n;
 }
 }

 return n;
}

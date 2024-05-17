size_t ACodec::countBuffersOwnedByComponent(OMX_U32 portIndex) const {
 size_t n = 0;

 for (size_t i = 0; i < mBuffers[portIndex].size(); ++i) {
 const BufferInfo &info = mBuffers[portIndex].itemAt(i);

 if (info.mStatus == BufferInfo::OWNED_BY_COMPONENT) {
 ++n;
 }
 }

 return n;
}

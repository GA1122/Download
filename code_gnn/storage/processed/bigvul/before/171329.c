OMXCodec::BufferInfo *OMXCodec::findEmptyInputBuffer() {
 Vector<BufferInfo> *infos = &mPortBuffers[kPortIndexInput];
 for (size_t i = 0; i < infos->size(); ++i) {
 BufferInfo *info = &infos->editItemAt(i);

 if (info->mStatus == OWNED_BY_US) {
 return info;
 }
 }

    TRESPASS();
}

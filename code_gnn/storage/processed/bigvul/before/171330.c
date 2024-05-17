OMXCodec::BufferInfo *OMXCodec::findInputBufferByDataPointer(void *ptr) {
 Vector<BufferInfo> *infos = &mPortBuffers[kPortIndexInput];
 for (size_t i = 0; i < infos->size(); ++i) {
 BufferInfo *info = &infos->editItemAt(i);

 if (info->mData == ptr) {
            CODEC_LOGV(
 "input buffer data ptr = %p, buffer_id = %u",
                    ptr,
                    info->mBuffer);

 return info;
 }
 }

    TRESPASS();
}

ssize_t AMediaCodec_dequeueOutputBuffer(AMediaCodec *mData,
 AMediaCodecBufferInfo *info, int64_t timeoutUs) {
 size_t idx;
 size_t offset;
 size_t size;
 uint32_t flags;
 int64_t presentationTimeUs;
 status_t ret = mData->mCodec->dequeueOutputBuffer(&idx, &offset, &size, &presentationTimeUs,
 &flags, timeoutUs);
    requestActivityNotification(mData);
 switch (ret) {
 case OK:
            info->offset = offset;
            info->size = size;
            info->flags = flags;
            info->presentationTimeUs = presentationTimeUs;
 return idx;
 case -EAGAIN:
 return AMEDIACODEC_INFO_TRY_AGAIN_LATER;
 case android::INFO_FORMAT_CHANGED:
 return AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED;
 case INFO_OUTPUT_BUFFERS_CHANGED:
 return AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED;
 default:
 break;
 }
 return translate_error(ret);
}

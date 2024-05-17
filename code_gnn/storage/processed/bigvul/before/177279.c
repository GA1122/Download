bool ACodec::BaseState::onOMXMessage(const sp<AMessage> &msg) {
 int32_t type;
    CHECK(msg->findInt32("type", &type));

 switch (type) {
 case omx_message::EVENT:
 {
 int32_t event, data1, data2;
            CHECK(msg->findInt32("event", &event));
            CHECK(msg->findInt32("data1", &data1));
            CHECK(msg->findInt32("data2", &data2));

 if (event == OMX_EventCmdComplete
 && data1 == OMX_CommandFlush
 && data2 == (int32_t)OMX_ALL) {

 return true;
 }

 return onOMXEvent(
 static_cast<OMX_EVENTTYPE>(event),
 static_cast<OMX_U32>(data1),
 static_cast<OMX_U32>(data2));
 }

 case omx_message::EMPTY_BUFFER_DONE:
 {
            IOMX::buffer_id bufferID;
 int32_t fenceFd;

            CHECK(msg->findInt32("buffer", (int32_t*)&bufferID));
            CHECK(msg->findInt32("fence_fd", &fenceFd));

 return onOMXEmptyBufferDone(bufferID, fenceFd);
 }

 case omx_message::FILL_BUFFER_DONE:
 {
            IOMX::buffer_id bufferID;
            CHECK(msg->findInt32("buffer", (int32_t*)&bufferID));

 int32_t rangeOffset, rangeLength, flags, fenceFd;
 int64_t timeUs;

            CHECK(msg->findInt32("range_offset", &rangeOffset));
            CHECK(msg->findInt32("range_length", &rangeLength));
            CHECK(msg->findInt32("flags", &flags));
            CHECK(msg->findInt64("timestamp", &timeUs));
            CHECK(msg->findInt32("fence_fd", &fenceFd));

 return onOMXFillBufferDone(
                    bufferID,
 (size_t)rangeOffset, (size_t)rangeLength,
 (OMX_U32)flags,
                    timeUs,
                    fenceFd);
 }

 case omx_message::FRAME_RENDERED:
 {
 int64_t mediaTimeUs, systemNano;

            CHECK(msg->findInt64("media_time_us", &mediaTimeUs));
            CHECK(msg->findInt64("system_nano", &systemNano));

 return onOMXFrameRendered(
                    mediaTimeUs, systemNano);
 }

 default:
            ALOGE("Unexpected message type: %d", type);
 return false;
 }
}

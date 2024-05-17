void Camera3Device::returnOutputBuffers(
 const camera3_stream_buffer_t *outputBuffers, size_t numBuffers,
 nsecs_t timestamp) {
 for (size_t i = 0; i < numBuffers; i++)
 {
 Camera3Stream *stream = Camera3Stream::cast(outputBuffers[i].stream);
 status_t res = stream->returnBuffer(outputBuffers[i], timestamp);
 if (res != OK) {
            ALOGE("Can't return buffer to its stream: %s (%d)",
                strerror(-res), res);
 }
 }
}

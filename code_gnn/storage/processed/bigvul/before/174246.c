status_t Camera3Device::prepare(int streamId) {
 return prepare(camera3::Camera3StreamInterface::ALLOCATE_PIPELINE_MAX, streamId);
}

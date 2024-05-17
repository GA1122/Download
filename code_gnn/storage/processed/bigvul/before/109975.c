AudioOutputDevice::~AudioOutputDevice() {
  CHECK_EQ(0, stream_id_);
}

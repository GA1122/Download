PlatformSensorWin::~PlatformSensorWin() {
  sensor_reader_->SetClient(nullptr);
  sensor_thread_runner_->DeleteSoon(FROM_HERE, sensor_reader_);
}

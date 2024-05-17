PlatformSensor::~PlatformSensor() {
  if (provider_)
    provider_->RemoveSensor(GetType(), this);
}

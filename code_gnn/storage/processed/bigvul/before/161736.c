void PlatformSensor::UpdateSharedBufferAndNotifyClients(
    const SensorReading& reading) {
  UpdateSharedBuffer(reading);
  task_runner_->PostTask(FROM_HERE,
                         base::Bind(&PlatformSensor::NotifySensorReadingChanged,
                                    weak_factory_.GetWeakPtr()));
 }

bool PlatformSensorAmbientLightMac::StartSensor(
    const PlatformSensorConfiguration& configuration) {

  light_sensor_service_.reset(IOServiceGetMatchingService(
      kIOMasterPortDefault, IOServiceMatching("AppleLMUController")));

  if (!light_sensor_service_)
    return false;

  light_sensor_port_.reset(IONotificationPortCreate(kIOMasterPortDefault));
  if (!light_sensor_port_.is_valid())
    return false;

  IONotificationPortSetDispatchQueue(
      light_sensor_port_.get(),
      dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0));

  kern_return_t kr = IOServiceAddInterestNotification(
      light_sensor_port_.get(), light_sensor_service_, kIOGeneralInterest,
      IOServiceCallback, this, light_sensor_notification_.InitializeInto());
  if (kr != KERN_SUCCESS)
    return false;

  kr = IOServiceAddInterestNotification(
      light_sensor_port_.get(), light_sensor_service_, kIOBusyInterest,
      IOServiceCallback, this,
      light_sensor_busy_notification_.InitializeInto());
  if (kr != KERN_SUCCESS)
    return false;

  kr = IOServiceOpen(light_sensor_service_, mach_task_self(), 0,
                     light_sensor_object_.InitializeInto());
  if (kr != KERN_SUCCESS)
    return false;

  bool success = ReadAndUpdate();
  if (!success)
    StopSensor();

  return success;
}

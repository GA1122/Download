void CloudPolicySubsystem::ScheduleServiceInitialization(
    int64 delay_milliseconds) {
  if (device_management_service_.get())
    device_management_service_->ScheduleInitialization(delay_milliseconds);
}

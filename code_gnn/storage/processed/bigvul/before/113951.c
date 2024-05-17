void RegistrationManager::TryRegisterId(const invalidation::ObjectId& id,
                                        bool is_retry) {
  DCHECK(CalledOnValidThread());
  RegistrationStatusMap::const_iterator it = registration_statuses_.find(id);
  if (it == registration_statuses_.end()) {
    DLOG(FATAL) << "TryRegisterId called on " << ObjectIdToString(id)
                << " which is not in the registration map";
    return;
  }
  RegistrationStatus* status = it->second;
  if (!status->enabled) {
    return;
  }
  status->last_registration_attempt = base::Time::Now();
  if (is_retry) {
    DCHECK(!status->last_registration_request.is_null());
    status->delay =
        (status->last_registration_request -
         status->last_registration_attempt) +
        status->next_delay;
    base::TimeDelta delay =
        (status->delay <= base::TimeDelta()) ?
        base::TimeDelta() : status->delay;
    DVLOG(2) << "Registering "
             << ObjectIdToString(id) << " in "
             << delay.InMilliseconds() << " ms";
    status->registration_timer.Stop();
    status->registration_timer.Start(FROM_HERE,
        delay, status, &RegistrationManager::RegistrationStatus::DoRegister);
    double next_delay_seconds =
        CalculateBackoff(static_cast<double>(status->next_delay.InSeconds()),
                         kInitialRegistrationDelaySeconds,
                         kMinRegistrationDelaySeconds,
                         kMaxRegistrationDelaySeconds,
                         kRegistrationDelayExponent,
                         GetJitter(),
                         kRegistrationDelayMaxJitter);
    status->next_delay =
        base::TimeDelta::FromSeconds(static_cast<int64>(next_delay_seconds));
    DVLOG(2) << "New next delay for "
             << ObjectIdToString(id) << " is "
             << status->next_delay.InSeconds() << " seconds";
  } else {
    DVLOG(2) << "Not a retry -- registering "
             << ObjectIdToString(id) << " immediately";
    status->delay = base::TimeDelta();
    status->next_delay = base::TimeDelta();
    status->DoRegister();
  }
}

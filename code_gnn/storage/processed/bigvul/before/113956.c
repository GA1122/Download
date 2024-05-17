void ExpectPendingRegistrations(
    const ObjectIdSet& expected_pending_ids,
    double expected_delay_seconds,
    const RegistrationManager::PendingRegistrationMap& pending_registrations) {
  ObjectIdSet pending_ids;
  for (RegistrationManager::PendingRegistrationMap::const_iterator it =
           pending_registrations.begin(); it != pending_registrations.end();
       ++it) {
    SCOPED_TRACE(ObjectIdToString(it->first));
    pending_ids.insert(it->first);
    base::TimeDelta offset =
        it->second.last_registration_request -
        it->second.registration_attempt;
    base::TimeDelta expected_delay =
        base::TimeDelta::FromSeconds(
            static_cast<int64>(expected_delay_seconds)) + offset;
    EXPECT_EQ(expected_delay, it->second.delay)
        << expected_delay.InMicroseconds()
        << ", " << it->second.delay.InMicroseconds();
    if (it->second.delay <= base::TimeDelta()) {
      EXPECT_EQ(base::TimeDelta(), it->second.actual_delay);
    } else {
      EXPECT_EQ(it->second.actual_delay, it->second.delay);
    }
  }
  EXPECT_EQ(expected_pending_ids, pending_ids);
}

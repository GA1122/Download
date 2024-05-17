bool HttpResponseHeaders::RequiresValidation(const Time& request_time,
                                             const Time& response_time,
                                             const Time& current_time) const {
  TimeDelta lifetime =
      GetFreshnessLifetime(response_time);
  if (lifetime == TimeDelta())
    return true;

  return lifetime <= GetCurrentAge(request_time, response_time, current_time);
}

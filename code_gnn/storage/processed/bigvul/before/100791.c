TimeDelta HttpResponseHeaders::GetCurrentAge(const Time& request_time,
                                             const Time& response_time,
                                             const Time& current_time) const {
  Time date_value;
  if (!GetDateValue(&date_value))
    date_value = response_time;

  TimeDelta age_value;
  GetAgeValue(&age_value);

  TimeDelta apparent_age = std::max(TimeDelta(), response_time - date_value);
  TimeDelta corrected_received_age = std::max(apparent_age, age_value);
  TimeDelta response_delay = response_time - request_time;
  TimeDelta corrected_initial_age = corrected_received_age + response_delay;
  TimeDelta resident_time = current_time - response_time;
  TimeDelta current_age = corrected_initial_age + resident_time;

  return current_age;
}

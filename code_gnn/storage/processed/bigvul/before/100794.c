TimeDelta HttpResponseHeaders::GetFreshnessLifetime(
    const Time& response_time) const {
  if (HasHeaderValue("cache-control", "no-cache") ||
      HasHeaderValue("cache-control", "no-store") ||
      HasHeaderValue("pragma", "no-cache") ||
      HasHeaderValue("vary", "*"))   
    return TimeDelta();   


  TimeDelta max_age_value;
  if (GetMaxAgeValue(&max_age_value))
    return max_age_value;

  Time date_value;
  if (!GetDateValue(&date_value))
    date_value = response_time;

  Time expires_value;
  if (GetExpiresValue(&expires_value)) {
    if (expires_value > date_value)
      return expires_value - date_value;

    return TimeDelta();   
  }

  if ((response_code_ == 200 || response_code_ == 203 ||
       response_code_ == 206) &&
      !HasHeaderValue("cache-control", "must-revalidate")) {
    Time last_modified_value;
    if (GetLastModifiedValue(&last_modified_value)) {
      if (last_modified_value <= date_value)
        return (date_value - last_modified_value) / 10;
    }
  }

  if (response_code_ == 300 || response_code_ == 301 || response_code_ == 410)
    return TimeDelta::FromMicroseconds(kint64max);

  return TimeDelta();   
}

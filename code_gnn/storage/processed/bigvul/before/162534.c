static double CurrentAge(const ResourceResponse& response,
                         double response_timestamp) {
  double date_value = response.Date();
  double apparent_age = std::isfinite(date_value)
                            ? std::max(0., response_timestamp - date_value)
                            : 0;
  double age_value = response.Age();
  double corrected_received_age = std::isfinite(age_value)
                                      ? std::max(apparent_age, age_value)
                                      : apparent_age;
  double resident_time = CurrentTime() - response_timestamp;
  return corrected_received_age + resident_time;
}

void LowBatteryObserver::PowerChanged(const PowerSupplyStatus& power_status) {
  const base::TimeDelta limit_min = base::TimeDelta::FromMinutes(15);
  const base::TimeDelta limit_max = base::TimeDelta::FromMinutes(30);
  const base::TimeDelta critical = base::TimeDelta::FromMinutes(5);

  base::TimeDelta remaining =
      base::TimeDelta::FromSeconds(power_status.battery_seconds_to_empty);

  bool line_power = power_status.line_power_on ||
                    remaining == base::TimeDelta();

  bool urgent = !line_power &&
      (notification_.urgent() || remaining <= critical);


  if (notification_.visible()) {
    if (line_power || remaining > limit_max) {
      Hide();
    } else if (remaining.InMinutes() != remaining_) {
      Show(remaining, urgent);
    }
  } else {
    if (!line_power && remaining <= limit_min) {
      Show(remaining, urgent);
    }
  }
}

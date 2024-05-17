base::TimeDelta GetWakeUpDuration() {
  int duration_ms;
  if (!base::StringToInt(base::GetFieldTrialParamValue(kWakeUpThrottlingTrial,
                                                       kWakeUpDurationParam),
                         &duration_ms))
    return kDefaultWakeUpDuration;
  return base::TimeDelta::FromMilliseconds(duration_ms);
}

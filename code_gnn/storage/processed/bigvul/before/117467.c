void TestingAutomationProvider::GetBatteryInfo(DictionaryValue* args,
                                               IPC::Message* reply_message) {
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);

  return_value->SetBoolean("battery_is_present",
                           global_power_status.battery_is_present);
  return_value->SetBoolean("line_power_on", global_power_status.line_power_on);
  if (global_power_status.battery_is_present) {
    return_value->SetBoolean("battery_fully_charged",
                             global_power_status.battery_is_full);
    return_value->SetDouble("battery_percentage",
                            global_power_status.battery_percentage);
    if (global_power_status.line_power_on) {
      int64 time = global_power_status.battery_seconds_to_full;
      if (time > 0 || global_power_status.battery_is_full)
        return_value->SetInteger("battery_seconds_to_full", time);
    } else {
      int64 time = global_power_status.battery_seconds_to_empty;
      if (time > 0)
        return_value->SetInteger("battery_seconds_to_empty", time);
    }
  }

  AutomationJSONReply(this, reply_message).SendSuccess(return_value.get());
}

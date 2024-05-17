void TestingAutomationProvider::SetTimezone(DictionaryValue* args,
                                            IPC::Message* reply_message) {
  std::string timezone_id;
  if (!args->GetString("timezone", &timezone_id)) {
    AutomationJSONReply(this, reply_message).SendError(
        "Invalid or missing args.");
    return;
  }

  icu::TimeZone* timezone =
      icu::TimeZone::createTimeZone(icu::UnicodeString::fromUTF8(timezone_id));
  chromeos::system::TimezoneSettings::GetInstance()->SetTimezone(*timezone);
  delete timezone;
  AutomationJSONReply(this, reply_message).SendSuccess(NULL);
}

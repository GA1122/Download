void TestingAutomationProvider::GetTimeInfo(Browser* browser,
                                            DictionaryValue* args,
                                            IPC::Message* reply_message) {
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  base::Time time(base::Time::Now());
  bool use_24hour_clock = browser && browser->profile()->GetPrefs()->GetBoolean(
      prefs::kUse24HourClock);
  base::HourClockType hour_clock_type =
      use_24hour_clock ? base::k24HourClock : base::k12HourClock;
  string16 display_time = base::TimeFormatTimeOfDayWithHourClockType(
      time, hour_clock_type, base::kDropAmPm);
  icu::UnicodeString unicode;
  chromeos::system::TimezoneSettings::GetInstance()->GetTimezone().getID(
      unicode);
  std::string timezone;
  UTF16ToUTF8(unicode.getBuffer(), unicode.length(), &timezone);
  return_value->SetString("display_time", display_time);
  return_value->SetString("display_date", base::TimeFormatFriendlyDate(time));
  return_value->SetString("timezone", timezone);
  AutomationJSONReply(this, reply_message).SendSuccess(return_value.get());
}

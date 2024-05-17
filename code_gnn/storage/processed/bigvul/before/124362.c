void BrowserEventRouter::DispatchSimpleBrowserEvent(
    Profile* profile, const int window_id, const char* event_name) {
  if (!profile_->IsSameProfile(profile))
    return;

  scoped_ptr<ListValue> args(new ListValue());
  args->Append(Value::CreateIntegerValue(window_id));

  DispatchEvent(profile, event_name, args.Pass(),
                 EventRouter::USER_GESTURE_UNKNOWN);
 }

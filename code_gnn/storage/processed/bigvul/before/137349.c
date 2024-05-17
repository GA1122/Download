bool MockInputMethod::OnUntranslatedIMEMessage(const base::NativeEvent& event,
                                               NativeEventResult* result) {
  if (result)
    *result = NativeEventResult();
  return false;
}

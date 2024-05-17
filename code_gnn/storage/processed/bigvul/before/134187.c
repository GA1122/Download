base::NativeEvent CreateNoopEvent() {
  MSG event = { NULL };
  event.message = WM_USER + 310;
  return event;
}

base::TimeDelta EventTimeFromNative(const base::NativeEvent& native_event) {
  return base::TimeDelta::FromMilliseconds(native_event.time);
}

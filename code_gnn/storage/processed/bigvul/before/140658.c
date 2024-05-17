void OutOfProcessInstance::ScrollToX(int x) {
  pp::VarDictionary position;
  position.Set(kType, kJSSetScrollPositionType);
  position.Set(kJSPositionX, pp::Var(x / device_scale_));
  PostMessage(position);
}

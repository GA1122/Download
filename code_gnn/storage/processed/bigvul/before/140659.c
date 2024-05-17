void OutOfProcessInstance::ScrollToY(int y) {
  pp::VarDictionary position;
  position.Set(kType, kJSSetScrollPositionType);
  position.Set(kJSPositionY, pp::Var(y / device_scale_));
  PostMessage(position);
}

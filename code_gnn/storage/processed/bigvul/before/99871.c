void WebPluginDelegatePepper::SetFocus() {
  NPPepperEvent npevent;

  npevent.type = NPEventType_Focus;
  npevent.size = sizeof(npevent);
  npevent.timeStampSeconds = 0.0;
  npevent.u.focus.value = 1;
  instance()->NPP_HandleEvent(&npevent);
}

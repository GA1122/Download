bool WebPluginDelegatePepper::HandleInputEvent(const WebInputEvent& event,
                                               WebCursorInfo* cursor_info) {
  NPPepperEvent npevent;

  npevent.type = ConvertEventTypes(event.type);
  npevent.size = sizeof(npevent);
  npevent.timeStampSeconds = event.timeStampSeconds;
  switch (npevent.type) {
    case NPEventType_Undefined:
      return false;
    case NPEventType_MouseDown:
    case NPEventType_MouseUp:
    case NPEventType_MouseMove:
    case NPEventType_MouseEnter:
    case NPEventType_MouseLeave:
      BuildMouseEvent(&event, &npevent);
      break;
    case NPEventType_MouseWheel:
      BuildMouseWheelEvent(&event, &npevent);
      break;
    case NPEventType_RawKeyDown:
    case NPEventType_KeyDown:
    case NPEventType_KeyUp:
      BuildKeyEvent(&event, &npevent);
      break;
    case NPEventType_Char:
      BuildCharEvent(&event, &npevent);
      break;
    case NPEventType_Minimize:
    case NPEventType_Focus:
    case NPEventType_Device:
      break;
  }
  bool rv = instance()->NPP_HandleEvent(&npevent) != 0;
  if (cursor_.get())
    *cursor_info = *cursor_;
  return rv;
}

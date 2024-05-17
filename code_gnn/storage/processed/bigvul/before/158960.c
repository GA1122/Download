void PDFiumEngine::HandleLongPress(const pp::TouchInputEvent& event) {
  pp::FloatPoint fp =
      event.GetTouchByIndex(PP_TOUCHLIST_TYPE_TARGETTOUCHES, 0).position();
  pp::Point point;
  point.set_x(fp.x());
  point.set_y(fp.y());

  pp::MouseInputEvent mouse_event(
      client_->GetPluginInstance(), PP_INPUTEVENT_TYPE_MOUSEDOWN,
      event.GetTimeStamp(), event.GetModifiers(),
      PP_INPUTEVENT_MOUSEBUTTON_LEFT, point, 2, point);

  OnMouseDown(mouse_event);
}

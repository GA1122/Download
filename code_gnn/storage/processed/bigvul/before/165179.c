static WebMouseEvent CreateMouseEvent(DragData* drag_data) {
  WebMouseEvent result(
      WebInputEvent::kMouseMove,
      WebFloatPoint(drag_data->ClientPosition().X(),
                    drag_data->ClientPosition().Y()),
      WebFloatPoint(drag_data->GlobalPosition().X(),
                    drag_data->GlobalPosition().Y()),
      WebPointerProperties::Button::kLeft, 0,
      static_cast<WebInputEvent::Modifiers>(drag_data->GetModifiers()),
      CurrentTimeTicksInSeconds());
  result.SetFrameScale(1);
  return result;
}

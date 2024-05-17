bool GenerateTouchPoints(
    blink::WebTouchEvent* event,
    blink::WebInputEvent::Type type,
    const base::flat_map<int, blink::WebTouchPoint>& points,
    const blink::WebTouchPoint& changing) {
  event->touches_length = 1;
  event->touches[0] = changing;
  for (auto& it : points) {
    if (it.first == changing.id)
      continue;
    if (event->touches_length == blink::WebTouchEvent::kTouchesLengthCap)
      return false;
    event->touches[event->touches_length] = it.second;
    event->touches[event->touches_length].state =
        blink::WebTouchPoint::kStateStationary;
    event->touches_length++;
  }
  if (type != blink::WebInputEvent::kUndefined) {
    event->touches[0].state = type == blink::WebInputEvent::kTouchCancel
                                  ? blink::WebTouchPoint::kStateCancelled
                                  : blink::WebTouchPoint::kStateReleased;
    event->SetType(type);
  } else if (points.find(changing.id) == points.end()) {
    event->touches[0].state = blink::WebTouchPoint::kStatePressed;
    event->SetType(blink::WebInputEvent::kTouchStart);
  } else {
    event->touches[0].state = blink::WebTouchPoint::kStateMoved;
    event->SetType(blink::WebInputEvent::kTouchMove);
  }
  return true;
}

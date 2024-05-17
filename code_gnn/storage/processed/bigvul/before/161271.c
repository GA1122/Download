void InputHandler::DispatchTouchEvent(
    const std::string& event_type,
    std::unique_ptr<Array<Input::TouchPoint>> touch_points,
    protocol::Maybe<int> maybe_modifiers,
    protocol::Maybe<double> maybe_timestamp,
    std::unique_ptr<DispatchTouchEventCallback> callback) {
  blink::WebInputEvent::Type type = GetTouchEventType(event_type);
  if (type == blink::WebInputEvent::kUndefined) {
    callback->sendFailure(Response::InvalidParams(
        base::StringPrintf("Unexpected event type '%s'", event_type.c_str())));
    return;
  }

  int modifiers = GetEventModifiers(
      maybe_modifiers.fromMaybe(blink::WebInputEvent::kNoModifiers), false,
      false, 0);
  double timestamp = GetEventTimestamp(maybe_timestamp);

  if ((type == blink::WebInputEvent::kTouchStart ||
       type == blink::WebInputEvent::kTouchMove) &&
      touch_points->length() == 0) {
    callback->sendFailure(Response::InvalidParams(
        "TouchStart and TouchMove must have at least one touch point."));
    return;
  }
  if ((type == blink::WebInputEvent::kTouchEnd ||
       type == blink::WebInputEvent::kTouchCancel) &&
      touch_points->length() > 0) {
    callback->sendFailure(Response::InvalidParams(
        "TouchEnd and TouchCancel must not have any touch points."));
    return;
  }
  if (type == blink::WebInputEvent::kTouchStart && !touch_points_.empty()) {
    callback->sendFailure(Response::InvalidParams(
        "Must have no prior active touch points to start a new touch."));
    return;
  }
  if (type != blink::WebInputEvent::kTouchStart && touch_points_.empty()) {
    callback->sendFailure(Response::InvalidParams(
        "Must send a TouchStart first to start a new touch."));
    return;
  }

  base::flat_map<int, blink::WebTouchPoint> points;
  size_t with_id = 0;
  for (size_t i = 0; i < touch_points->length(); ++i) {
    Input::TouchPoint* point = touch_points->get(i);
    int id = point->GetId(i);
    if (point->HasId())
      with_id++;
    points[id].id = id;
    points[id].radius_x = point->GetRadiusX(1.0);
    points[id].radius_y = point->GetRadiusY(1.0);
    points[id].rotation_angle = point->GetRotationAngle(0.0);
    points[id].force = point->GetForce(1.0);
    points[id].pointer_type = blink::WebPointerProperties::PointerType::kTouch;
    points[id].SetPositionInWidget(point->GetX() * page_scale_factor_,
                                   point->GetY() * page_scale_factor_);
    points[id].SetPositionInScreen(point->GetX() * page_scale_factor_,
                                   point->GetY() * page_scale_factor_);
  }
  if (with_id > 0 && with_id < touch_points->length()) {
    callback->sendFailure(Response::InvalidParams(
        "All or none of the provided TouchPoints must supply ids."));
    return;
  }

  if (!host_ || !host_->GetRenderWidgetHost()) {
    callback->sendFailure(Response::InternalError());
    return;
  }

  std::vector<blink::WebTouchEvent> events;
  bool ok = true;
  for (auto& id_point : points) {
    if (touch_points_.find(id_point.first) != touch_points_.end())
      continue;
    events.emplace_back(type, modifiers, timestamp);
    ok &= GenerateTouchPoints(&events.back(), blink::WebInputEvent::kUndefined,
                              touch_points_, id_point.second);
    touch_points_.insert(id_point);
  }
  for (auto& id_point : points) {
    DCHECK(touch_points_.find(id_point.first) != touch_points_.end());
    if (touch_points_[id_point.first].PositionInWidget() ==
        id_point.second.PositionInWidget()) {
      continue;
    }
    events.emplace_back(type, modifiers, timestamp);
    ok &= GenerateTouchPoints(&events.back(), blink::WebInputEvent::kUndefined,
                              touch_points_, id_point.second);
    touch_points_[id_point.first] = id_point.second;
  }
  if (type != blink::WebInputEvent::kTouchCancel)
    type = blink::WebInputEvent::kTouchEnd;
  for (auto it = touch_points_.begin(); it != touch_points_.end();) {
    if (points.find(it->first) != points.end()) {
      it++;
      continue;
    }
    events.emplace_back(type, modifiers, timestamp);
    ok &= GenerateTouchPoints(&events.back(), type, touch_points_, it->second);
    it = touch_points_.erase(it);
  }
  if (!ok) {
    callback->sendFailure(Response::Error(
        base::StringPrintf("Exceeded maximum touch points limit of %d",
                           blink::WebTouchEvent::kTouchesLengthCap)));
    return;
  }

  if (events.empty()) {
    callback->sendSuccess();
    return;
  }

  host_->GetRenderWidgetHost()->Focus();
  host_->GetRenderWidgetHost()->GetTouchEmulator()->Enable(
      TouchEmulator::Mode::kInjectingTouchEvents,
      ui::GestureProviderConfigType::CURRENT_PLATFORM);
  base::OnceClosure closure = base::BindOnce(
      &DispatchTouchEventCallback::sendSuccess, std::move(callback));
  for (size_t i = 0; i < events.size(); i++) {
    events[i].dispatch_type =
        events[i].GetType() == blink::WebInputEvent::kTouchCancel
            ? blink::WebInputEvent::kEventNonBlocking
            : blink::WebInputEvent::kBlocking;
    events[i].moved_beyond_slop_region = true;
    events[i].unique_touch_event_id = ui::GetNextTouchEventId();
    host_->GetRenderWidgetHost()->GetTouchEmulator()->InjectTouchEvent(
        events[i],
        i == events.size() - 1 ? std::move(closure) : base::OnceClosure());
  }
}

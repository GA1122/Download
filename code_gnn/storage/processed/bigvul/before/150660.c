InputHandlerProxy::RouteToTypeSpecificHandler(
    const WebInputEvent& event,
    const LatencyInfo& original_latency_info) {
  DCHECK(input_handler_);

  if (force_input_to_main_thread_)
    return DID_NOT_HANDLE;

  if (IsGestureScroll(event.GetType()) &&
      (snap_fling_controller_->FilterEventForSnap(
          GestureScrollEventType(event.GetType())))) {
    return DROP_EVENT;
  }

  switch (event.GetType()) {
    case WebInputEvent::kMouseWheel:
      return HandleMouseWheel(static_cast<const WebMouseWheelEvent&>(event));

    case WebInputEvent::kGestureScrollBegin:
      return HandleGestureScrollBegin(
          static_cast<const WebGestureEvent&>(event));

    case WebInputEvent::kGestureScrollUpdate:
      return HandleGestureScrollUpdate(
          static_cast<const WebGestureEvent&>(event));

    case WebInputEvent::kGestureScrollEnd:
      return HandleGestureScrollEnd(static_cast<const WebGestureEvent&>(event));

    case WebInputEvent::kGesturePinchBegin: {
      DCHECK(!gesture_pinch_in_progress_);
      input_handler_->PinchGestureBegin();
      gesture_pinch_in_progress_ = true;
      return DID_HANDLE;
    }

    case WebInputEvent::kGesturePinchEnd: {
      DCHECK(gesture_pinch_in_progress_);
      gesture_pinch_in_progress_ = false;
      const WebGestureEvent& gesture_event =
          static_cast<const WebGestureEvent&>(event);
      input_handler_->PinchGestureEnd(
          gfx::ToFlooredPoint(gesture_event.PositionInWidget()),
          gesture_event.SourceDevice() == blink::WebGestureDevice::kTouchpad);
      return DID_HANDLE;
    }

    case WebInputEvent::kGesturePinchUpdate: {
      DCHECK(gesture_pinch_in_progress_);
      const WebGestureEvent& gesture_event =
          static_cast<const WebGestureEvent&>(event);
      input_handler_->PinchGestureUpdate(
          gesture_event.data.pinch_update.scale,
          gfx::ToFlooredPoint(gesture_event.PositionInWidget()));
      return DID_HANDLE;
    }

    case WebInputEvent::kTouchStart:
      return HandleTouchStart(static_cast<const WebTouchEvent&>(event));

    case WebInputEvent::kTouchMove:
      return HandleTouchMove(static_cast<const WebTouchEvent&>(event));

    case WebInputEvent::kTouchEnd:
      return HandleTouchEnd(static_cast<const WebTouchEvent&>(event));

    case WebInputEvent::kMouseDown: {
      const WebMouseEvent& mouse_event =
          static_cast<const WebMouseEvent&>(event);

      if (mouse_event.button == blink::WebMouseEvent::Button::kLeft) {
        CHECK(input_handler_);
        cc::InputHandlerPointerResult pointer_result =
            input_handler_->MouseDown(
                gfx::PointF(mouse_event.PositionInWidget()));
        if (pointer_result.type == cc::PointerResultType::kScrollbarScroll) {
          InjectScrollbarGestureScroll(WebInputEvent::Type::kGestureScrollBegin,
                                       mouse_event.PositionInWidget(),
                                       pointer_result, original_latency_info,
                                       mouse_event.TimeStamp());
          InjectScrollbarGestureScroll(
              WebInputEvent::Type::kGestureScrollUpdate,
              mouse_event.PositionInWidget(), pointer_result,
              original_latency_info, mouse_event.TimeStamp());

          return DROP_EVENT;
        }
      }

      return DID_NOT_HANDLE;
    }
    case WebInputEvent::kMouseUp: {
      const WebMouseEvent& mouse_event =
          static_cast<const WebMouseEvent&>(event);

      if (mouse_event.button == blink::WebMouseEvent::Button::kLeft) {
        CHECK(input_handler_);
        cc::InputHandlerPointerResult pointer_result = input_handler_->MouseUp(
            gfx::PointF(mouse_event.PositionInWidget()));
        if (pointer_result.type == cc::PointerResultType::kScrollbarScroll) {
          InjectScrollbarGestureScroll(WebInputEvent::Type::kGestureScrollEnd,
                                       mouse_event.PositionInWidget(),
                                       pointer_result, original_latency_info,
                                       mouse_event.TimeStamp());

          return DROP_EVENT;
        }
      }
      return DID_NOT_HANDLE;
    }
    case WebInputEvent::kMouseMove: {
      const WebMouseEvent& mouse_event =
          static_cast<const WebMouseEvent&>(event);
      CHECK(input_handler_);
      cc::InputHandlerPointerResult pointer_result =
          input_handler_->MouseMoveAt(
              gfx::Point(mouse_event.PositionInWidget().x,
                         mouse_event.PositionInWidget().y));
      if (pointer_result.type == cc::PointerResultType::kScrollbarScroll) {
        InjectScrollbarGestureScroll(WebInputEvent::Type::kGestureScrollUpdate,
                                     mouse_event.PositionInWidget(),
                                     pointer_result, original_latency_info,
                                     mouse_event.TimeStamp());

        return DROP_EVENT;
      }
      return DID_NOT_HANDLE;
    }
    case WebInputEvent::kMouseLeave: {
      CHECK(input_handler_);
      input_handler_->MouseLeave();
      return DID_NOT_HANDLE;
    }
    case WebInputEvent::kGestureFlingStart:
    case WebInputEvent::kGestureFlingCancel:
      NOTREACHED();
      break;

    default:
      break;
  }

  return DID_NOT_HANDLE;
}

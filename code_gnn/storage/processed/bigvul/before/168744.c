void RenderWidgetHostImpl::ForwardGestureEventWithLatencyInfo(
    const blink::WebGestureEvent& gesture_event,
    const ui::LatencyInfo& latency) {
  TRACE_EVENT1("input", "RenderWidgetHostImpl::ForwardGestureEvent", "type",
               WebInputEvent::GetName(gesture_event.GetType()));
  if (ShouldDropInputEvents())
    return;

  bool scroll_update_needs_wrapping = false;
  if (gesture_event.GetType() == blink::WebInputEvent::kGestureScrollBegin) {
    DCHECK(!is_in_gesture_scroll_[gesture_event.source_device]);
    is_in_gesture_scroll_[gesture_event.source_device] = true;
  } else if (gesture_event.GetType() ==
             blink::WebInputEvent::kGestureScrollEnd) {
    DCHECK(is_in_gesture_scroll_[gesture_event.source_device]);
    is_in_gesture_scroll_[gesture_event.source_device] = false;
    is_in_touchpad_gesture_fling_ = false;
  } else if (gesture_event.GetType() ==
             blink::WebInputEvent::kGestureFlingStart) {
    if (gesture_event.source_device ==
        blink::WebGestureDevice::kWebGestureDeviceTouchpad) {
      if (GetView()->IsInVR()) {
        DCHECK(is_in_gesture_scroll_[gesture_event.source_device]);

        is_in_gesture_scroll_[gesture_event.source_device] = false;
      } else if (GetView()->wheel_scroll_latching_enabled()) {
        DCHECK(is_in_gesture_scroll_[gesture_event.source_device]);

      } else {   

        DCHECK(!is_in_gesture_scroll_[gesture_event.source_device]);
      }

      is_in_touchpad_gesture_fling_ = true;
    } else {   
      DCHECK(is_in_gesture_scroll_[gesture_event.source_device]);
      is_in_gesture_scroll_[gesture_event.source_device] = false;
    }
  }

  scroll_update_needs_wrapping =
      gesture_event.GetType() == blink::WebInputEvent::kGestureScrollUpdate &&
      gesture_event.resending_plugin_id != -1 &&
      !is_in_gesture_scroll_[gesture_event.source_device];

  if (scroll_update_needs_wrapping) {
    ForwardGestureEventWithLatencyInfo(
        CreateScrollBeginForWrapping(gesture_event),
        ui::WebInputEventTraits::CreateLatencyInfoForWebGestureEvent(
            gesture_event));
  }

  if (delegate_->PreHandleGestureEvent(gesture_event))
    return;

  GestureEventWithLatencyInfo gesture_with_latency(gesture_event, latency);
  DispatchInputEventWithLatencyInfo(gesture_event,
                                    &gesture_with_latency.latency);
  input_router_->SendGestureEvent(gesture_with_latency);

  if (scroll_update_needs_wrapping) {
    ForwardGestureEventWithLatencyInfo(
        CreateScrollEndForWrapping(gesture_event),
        ui::WebInputEventTraits::CreateLatencyInfoForWebGestureEvent(
            gesture_event));
  }
}

void RendererSchedulerImpl::UpdateForInputEventOnCompositorThread(
    blink::WebInputEvent::Type type,
    InputEventState input_event_state) {
  base::AutoLock lock(any_thread_lock_);
  base::TimeTicks now = helper_.NowTicks();

  TRACE_EVENT2(TRACE_DISABLED_BY_DEFAULT("renderer.scheduler"),
               "RendererSchedulerImpl::UpdateForInputEventOnCompositorThread",
               "type", static_cast<int>(type), "input_event_state",
               InputEventStateToString(input_event_state));

  base::TimeDelta unused_policy_duration;
  UseCase previous_use_case =
      ComputeCurrentUseCase(now, &unused_policy_duration);
  bool was_awaiting_touch_start_response =
      any_thread().awaiting_touch_start_response;

  any_thread().user_model.DidStartProcessingInputEvent(type, now);
  any_thread().have_seen_input_since_navigation = true;

  if (input_event_state == InputEventState::EVENT_CONSUMED_BY_COMPOSITOR)
    any_thread().user_model.DidFinishProcessingInputEvent(now);

  switch (type) {
    case blink::WebInputEvent::kTouchStart:
      any_thread().awaiting_touch_start_response = true;
      any_thread().last_gesture_was_compositor_driven = false;
      any_thread().have_seen_a_potentially_blocking_gesture = true;
      any_thread().default_gesture_prevented = true;
      break;

    case blink::WebInputEvent::kTouchMove:
      if (any_thread().awaiting_touch_start_response &&
          GetCompositorThreadOnly().last_input_type ==
              blink::WebInputEvent::kTouchMove) {
        any_thread().awaiting_touch_start_response = false;
      }
      break;

    case blink::WebInputEvent::kGesturePinchUpdate:
    case blink::WebInputEvent::kGestureScrollUpdate:
      any_thread().last_gesture_was_compositor_driven =
          input_event_state == InputEventState::EVENT_CONSUMED_BY_COMPOSITOR;
      any_thread().awaiting_touch_start_response = false;
      any_thread().default_gesture_prevented = false;
      break;

    case blink::WebInputEvent::kGestureFlingCancel:
      any_thread().fling_compositor_escalation_deadline = base::TimeTicks();
      break;

    case blink::WebInputEvent::kGestureTapDown:
    case blink::WebInputEvent::kGestureShowPress:
    case blink::WebInputEvent::kGestureScrollEnd:
      break;

    case blink::WebInputEvent::kMouseDown:
      any_thread().last_gesture_was_compositor_driven = false;
      any_thread().default_gesture_prevented = true;
      break;

    case blink::WebInputEvent::kMouseMove:
      any_thread().last_gesture_was_compositor_driven =
          input_event_state == InputEventState::EVENT_CONSUMED_BY_COMPOSITOR;
      any_thread().awaiting_touch_start_response = false;
      break;

    case blink::WebInputEvent::kMouseWheel:
      any_thread().last_gesture_was_compositor_driven =
          input_event_state == InputEventState::EVENT_CONSUMED_BY_COMPOSITOR;
      any_thread().awaiting_touch_start_response = false;
      any_thread().have_seen_a_potentially_blocking_gesture = true;
      any_thread().default_gesture_prevented =
          !any_thread().last_gesture_was_compositor_driven;
      break;

    case blink::WebInputEvent::kUndefined:
      break;

    default:
      any_thread().awaiting_touch_start_response = false;
      break;
  }

  UseCase use_case = ComputeCurrentUseCase(now, &unused_policy_duration);

  if (use_case != previous_use_case ||
      was_awaiting_touch_start_response !=
          any_thread().awaiting_touch_start_response) {
    EnsureUrgentPolicyUpdatePostedOnMainThread(FROM_HERE);
  }
  GetCompositorThreadOnly().last_input_type = type;
}

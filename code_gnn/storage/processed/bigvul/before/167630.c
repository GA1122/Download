  cc::TouchAction GetEffectiveTouchActionForChild(
      RenderWidgetHostInputEventRouter* router,
      RenderWidgetHostViewBase* rwhv_root,
      RenderWidgetHostViewBase* rwhv_child,
      const gfx::Point& event_position) {
    InputEventAckWaiter ack_observer(
        rwhv_child->GetRenderWidgetHost(),
        base::BindRepeating([](content::InputEventAckSource source,
                               content::InputEventAckState state,
                               const blink::WebInputEvent& event) {
          return event.GetType() == blink::WebGestureEvent::kTouchStart ||
                 event.GetType() == blink::WebGestureEvent::kTouchMove ||
                 event.GetType() == blink::WebGestureEvent::kTouchEnd;
        }));

    ack_observer.Reset();
    SyntheticWebTouchEvent touch_event;
    int index = touch_event.PressPoint(event_position.x(), event_position.y());
    router->RouteTouchEvent(rwhv_root, &touch_event,
                            ui::LatencyInfo(ui::SourceEventType::TOUCH));
    ack_observer.Wait();
    base::Optional<cc::TouchAction> touch_action =
        static_cast<RenderWidgetHostImpl*>(rwhv_child->GetRenderWidgetHost())
            ->input_router()
            ->AllowedTouchAction();

    ack_observer.Reset();
    touch_event.MovePoint(index, 1, 1);
    router->RouteTouchEvent(rwhv_root, &touch_event,
                            ui::LatencyInfo(ui::SourceEventType::TOUCH));
    ack_observer.Wait();

    ack_observer.Reset();
    touch_event.ReleasePoint(index);
    router->RouteTouchEvent(rwhv_root, &touch_event,
                            ui::LatencyInfo(ui::SourceEventType::TOUCH));
    ack_observer.Wait();
    return touch_action.has_value() ? touch_action.value()
                                    : cc::kTouchActionAuto;
  }

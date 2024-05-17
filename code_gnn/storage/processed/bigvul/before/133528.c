bool OverscrollControllerAndroid::WillHandleGestureEvent(
    const blink::WebGestureEvent& event) {
  if (!enabled_)
    return false;

  if (!refresh_effect_)
    return false;

  if (is_fullscreen_)
    return false;

  if (glow_effect_ && glow_effect_->IsActive()) {
    if (glow_effect_->GetVisibleAlpha() > MinGlowAlphaToDisableRefresh())
      return false;
  }

  bool handled = false;
  switch (event.type) {
    case blink::WebInputEvent::GestureScrollBegin:
      refresh_effect_->OnScrollBegin();
      break;

    case blink::WebInputEvent::GestureScrollUpdate: {
      gfx::Vector2dF scroll_delta(event.data.scrollUpdate.deltaX,
                                  event.data.scrollUpdate.deltaY);
      scroll_delta.Scale(dpi_scale_);
      handled = refresh_effect_->WillHandleScrollUpdate(scroll_delta);
    } break;

    case blink::WebInputEvent::GestureScrollEnd:
      refresh_effect_->OnScrollEnd(gfx::Vector2dF());
      break;

    case blink::WebInputEvent::GestureFlingStart: {
      if (refresh_effect_->IsActive()) {
        gfx::Vector2dF scroll_velocity(event.data.flingStart.velocityX,
                                       event.data.flingStart.velocityY);
        scroll_velocity.Scale(dpi_scale_);
        refresh_effect_->OnScrollEnd(scroll_velocity);
        blink::WebGestureEvent& modified_event =
            const_cast<blink::WebGestureEvent&>(event);
        modified_event.data.flingStart.velocityX = .01f;
        modified_event.data.flingStart.velocityY = .01f;
      }
    } break;

    case blink::WebInputEvent::GesturePinchBegin:
      refresh_effect_->ReleaseWithoutActivation();
      break;

    default:
      break;
  }

  return handled;
}

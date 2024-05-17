void InputHandlerProxy::RecordMainThreadScrollingReasons(
    blink::WebGestureDevice device,
    uint32_t reasons) {
  static const char* kGestureHistogramName =
      "Renderer4.MainThreadGestureScrollReason";
  static const char* kWheelHistogramName =
      "Renderer4.MainThreadWheelScrollReason";

  if (device != blink::WebGestureDevice::kTouchpad &&
      device != blink::WebGestureDevice::kTouchscreen) {
    return;
  }

  DCHECK(
      !cc::MainThreadScrollingReason::HasNonCompositedScrollReasons(reasons));

  int32_t event_disposition_result =
      (device == blink::WebGestureDevice::kTouchpad ? mouse_wheel_result_
                                                    : touch_result_);
  if (event_disposition_result == DID_NOT_HANDLE) {
    reasons |= (device == blink::WebGestureDevice::kTouchpad
                    ? cc::MainThreadScrollingReason::kWheelEventHandlerRegion
                    : cc::MainThreadScrollingReason::kTouchEventHandlerRegion);
  }

  constexpr uint32_t kMainThreadScrollingReasonEnumMax =
      cc::MainThreadScrollingReason::kMainThreadScrollingReasonCount + 1;
  if (reasons == cc::MainThreadScrollingReason::kNotScrollingOnMain) {
    if (device == blink::WebGestureDevice::kTouchscreen) {
      UMA_HISTOGRAM_ENUMERATION(
          kGestureHistogramName,
          cc::MainThreadScrollingReason::kNotScrollingOnMain,
          kMainThreadScrollingReasonEnumMax);
    } else {
      UMA_HISTOGRAM_ENUMERATION(
          kWheelHistogramName,
          cc::MainThreadScrollingReason::kNotScrollingOnMain,
          kMainThreadScrollingReasonEnumMax);
    }
  }

  for (uint32_t i = 0;
       i < cc::MainThreadScrollingReason::kMainThreadScrollingReasonCount;
       ++i) {
    unsigned val = 1 << i;
    if (reasons & val) {
      if (val == cc::MainThreadScrollingReason::kHandlingScrollFromMainThread) {
        if (reasons & ~val)
          continue;
      }
      if (device == blink::WebGestureDevice::kTouchscreen) {
        UMA_HISTOGRAM_ENUMERATION(kGestureHistogramName, i + 1,
                                  kMainThreadScrollingReasonEnumMax);
      } else {
        UMA_HISTOGRAM_ENUMERATION(kWheelHistogramName, i + 1,
                                  kMainThreadScrollingReasonEnumMax);
      }
    }
  }
}

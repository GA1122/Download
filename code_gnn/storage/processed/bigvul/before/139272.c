VRDisplay::VRDisplay(NavigatorVR* navigator_vr,
                     device::mojom::blink::VRDisplayPtr display,
                     device::mojom::blink::VRDisplayClientRequest request)
    : ContextLifecycleObserver(navigator_vr->GetDocument()),
      navigator_vr_(navigator_vr),
      capabilities_(new VRDisplayCapabilities()),
      eye_parameters_left_(new VREyeParameters()),
      eye_parameters_right_(new VREyeParameters()),
      display_(std::move(display)),
      submit_frame_client_binding_(this),
      display_client_binding_(this, std::move(request)) {}

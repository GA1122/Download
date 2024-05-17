void VRDisplay::OnActivate(device::mojom::blink::VRDisplayEventReason reason) {
  AutoReset<bool> activating(&in_display_activate_, true);
  navigator_vr_->DispatchVREvent(VRDisplayEvent::Create(
      EventTypeNames::vrdisplayactivate, true, false, this, reason));
}

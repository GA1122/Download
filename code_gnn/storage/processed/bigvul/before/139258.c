void VRDisplay::OnChanged(device::mojom::blink::VRDisplayInfoPtr display) {
  Update(display);
}

GamepadHand MojoToGamepadHandedness(device::mojom::XRHandedness handedness) {
  switch (handedness) {
    case device::mojom::XRHandedness::LEFT:
      return GamepadHand::kLeft;
    case device::mojom::XRHandedness::RIGHT:
      return GamepadHand::kRight;
    case device::mojom::XRHandedness::NONE:
      return GamepadHand::kNone;
  }

  NOTREACHED();
}

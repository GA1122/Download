InputDeviceCapabilitiesConstants* DOMWindow::GetInputDeviceCapabilities() {
  if (!input_capabilities_)
    input_capabilities_ = new InputDeviceCapabilitiesConstants;
  return input_capabilities_;
}

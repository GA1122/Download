   OpenVRGamepadBuilder(vr::IVRSystem* vr_system,
                       uint32_t controller_id,
                       vr::VRControllerState_t controller_state,
                       device::mojom::XRHandedness handedness)
      : GamepadBuilder(GetGamepadId(vr_system, controller_id),
                       GamepadMapping::kXRStandard,
                       handedness),
        controller_state_(controller_state) {
    supported_buttons_ = vr_system->GetUint64TrackedDeviceProperty(
        controller_id, vr::Prop_SupportedButtons_Uint64);

    axes_data_ = GetAxesButtons(vr_system, controller_state_,
                                supported_buttons_, controller_id);
  }

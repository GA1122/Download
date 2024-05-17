  unsigned int CreateAndConnectMinimalGamepad(
      device::XrAxisType primary_axis_type) {
    uint64_t supported_buttons =
        device::XrButtonMaskFromId(device::XrButtonId::kAxisTrigger) |
        device::XrButtonMaskFromId(device::XrButtonId::kAxisPrimary);

    std::map<device::XrButtonId, unsigned int> axis_types = {
        {device::XrButtonId::kAxisPrimary, primary_axis_type},
        {device::XrButtonId::kAxisTrigger, device::XrAxisType::kTrigger},
    };

    return CreateAndConnectController(
        device::ControllerRole::kControllerRoleRight, axis_types,
        supported_buttons);
  }

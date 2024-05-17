  void UpdateControllerSupport(
      unsigned int controller_index,
      const std::map<device::XrButtonId, unsigned int>& axis_types,
      uint64_t supported_buttons) {
    auto controller_data = GetCurrentControllerData(controller_index);

    for (unsigned int i = 0; i < device::kMaxNumAxes; i++) {
      auto button_id = GetAxisId(i);
      auto it = axis_types.find(button_id);
      unsigned int new_axis_type = device::XrAxisType::kNone;
      if (it != axis_types.end())
        new_axis_type = it->second;
      controller_data.axis_data[i].axis_type = new_axis_type;
    }

    controller_data.supported_buttons = supported_buttons;

    UpdateControllerAndWait(controller_index, controller_data);
  }

  void ToggleTriggerButton(unsigned int index, device::XrButtonId button_id) {
    auto controller_data = GetCurrentControllerData(index);
    uint64_t button_mask = device::XrButtonMaskFromId(button_id);

    controller_data.packet_number++;
    controller_data.buttons_pressed ^= button_mask;
    controller_data.buttons_touched ^= button_mask;

    bool is_pressed = ((controller_data.buttons_pressed & button_mask) != 0);

    unsigned int axis_offset = device::XrAxisOffsetFromId(button_id);
    DCHECK(controller_data.axis_data[axis_offset].axis_type ==
           device::XrAxisType::kTrigger);
    controller_data.axis_data[axis_offset].x = is_pressed ? 1.0 : 0.0;
    UpdateControllerAndWait(index, controller_data);
  }

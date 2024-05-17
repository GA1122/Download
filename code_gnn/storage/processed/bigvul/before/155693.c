  device::XrButtonId GetAxisId(unsigned int offset) {
    return static_cast<device::XrButtonId>(device::XrButtonId::kAxisPrimary +
                                           offset);
  }

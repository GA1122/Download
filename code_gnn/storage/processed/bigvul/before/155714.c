  static std::string GetGamepadId(vr::IVRSystem* vr_system,
                                  uint32_t controller_id) {
    if (IsControllerHTCVive(vr_system, controller_id)) {
      return "htc-vive";
    }

    return "openvr";
  }

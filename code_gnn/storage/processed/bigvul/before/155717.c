  static bool IsControllerHTCVive(vr::IVRSystem* vr_system,
                                  uint32_t controller_id) {
    std::string model =
        GetOpenVRString(vr_system, vr::Prop_ModelNumber_String, controller_id);
    std::string manufacturer = GetOpenVRString(
        vr_system, vr::Prop_ManufacturerName_String, controller_id);

    return (manufacturer == "HTC") && (model.find("Vive") != std::string::npos);
  }

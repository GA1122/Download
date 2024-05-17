mojom::XRGamepadDataPtr OpenVRGamepadHelper::GetGamepadData(
    vr::IVRSystem* vr_system) {
  mojom::XRGamepadDataPtr ret = mojom::XRGamepadData::New();

  vr::TrackedDevicePose_t tracked_devices_poses[vr::k_unMaxTrackedDeviceCount];
  vr_system->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseSeated, 0.0f,
                                             tracked_devices_poses,
                                             vr::k_unMaxTrackedDeviceCount);
  for (uint32_t i = 0; i < vr::k_unMaxTrackedDeviceCount; ++i) {
    if (vr_system->GetTrackedDeviceClass(i) !=
        vr::TrackedDeviceClass_Controller)
      continue;

    vr::VRControllerState_t controller_state;
    bool have_state = vr_system->GetControllerState(i, &controller_state,
                                                    sizeof(controller_state));
    if (!have_state)
      continue;

    auto gamepad = mojom::XRGamepad::New();
    gamepad->controller_id = i;

    gamepad->timestamp = base::TimeTicks::Now();

    vr::ETrackedControllerRole hand =
        vr_system->GetControllerRoleForTrackedDeviceIndex(i);
    switch (hand) {
      case vr::TrackedControllerRole_Invalid:
        gamepad->hand = device::mojom::XRHandedness::NONE;
        break;
      case vr::TrackedControllerRole_LeftHand:
        gamepad->hand = device::mojom::XRHandedness::LEFT;
        break;
      case vr::TrackedControllerRole_RightHand:
        gamepad->hand = device::mojom::XRHandedness::RIGHT;
        break;
    }

    uint64_t supported_buttons = vr_system->GetUint64TrackedDeviceProperty(
        i, vr::Prop_SupportedButtons_Uint64);

    std::map<vr::EVRButtonId, GamepadBuilder::ButtonData> button_data_map =
        GetAxesButtons(vr_system, controller_state, supported_buttons, i);

    for (const auto& button_data_pair : button_data_map) {
      GamepadBuilder::ButtonData data = button_data_pair.second;

      gamepad->buttons.push_back(GetMojomGamepadButton(data));
      if (data.has_both_axes) {
        gamepad->axes.push_back(data.x_axis);
        gamepad->axes.push_back(data.y_axis);
      }
    }

    for (const auto& button : kWebVRButtonOrder) {
      GamepadButton data;
      if (TryGetGamepadButton(controller_state, supported_buttons, button,
                              &data)) {
        gamepad->buttons.push_back(GetMojomGamepadButton(data));
      }
    }

    const vr::TrackedDevicePose_t& pose = tracked_devices_poses[i];
    if (pose.bPoseIsValid) {
      const vr::HmdMatrix34_t& mat = pose.mDeviceToAbsoluteTracking;
      gfx::Transform transform(
          mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3], mat.m[1][0],
          mat.m[1][1], mat.m[1][2], mat.m[1][3], mat.m[2][0], mat.m[2][1],
          mat.m[2][2], mat.m[2][3], 0, 0, 0, 1);

      gfx::DecomposedTransform src_pose;
      gfx::DecomposeTransform(&src_pose, transform);
      auto dst_pose = mojom::VRPose::New();

      dst_pose->orientation = std::vector<float>(
          {src_pose.quaternion.x(), src_pose.quaternion.y(),
           src_pose.quaternion.z(), src_pose.quaternion.w()});
      dst_pose->position =
          std::vector<float>({src_pose.translate[0], src_pose.translate[1],
                              src_pose.translate[2]});
      dst_pose->angularVelocity = std::vector<float>(
          {pose.vAngularVelocity.v[0], pose.vAngularVelocity.v[1],
           pose.vAngularVelocity.v[2]});
      dst_pose->linearVelocity = std::vector<float>(
          {pose.vVelocity.v[0], pose.vVelocity.v[1], pose.vVelocity.v[2]});

      gamepad->pose = std::move(dst_pose);
    }

    ret->gamepads.push_back(std::move(gamepad));
  }

  return ret;
}

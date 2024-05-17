 base::Optional<Gamepad> OpenVRGamepadHelper::GetXRGamepad(
     vr::IVRSystem* vr_system,
     uint32_t controller_id,
     vr::VRControllerState_t controller_state,
     device::mojom::XRHandedness handedness) {
    OpenVRGamepadBuilder builder(vr_system, controller_id, controller_state,
                                 handedness);
  
  if (!builder.TryAddAxesButton(vr::k_EButton_SteamVR_Trigger))
//   if (!builder.TryAddAxesOrTriggerButton(vr::k_EButton_SteamVR_Trigger))
      return base::nullopt;
  
  if (!builder.TryAddNextUnusedAxesButton())
//   if (!builder.TryAddNextUnusedButtonWithAxes())
      return base::nullopt;
  
  if (!builder.TryAddButton(vr::k_EButton_Grip))
//   bool added_placeholder_grip = false;
//   if (!builder.TryAddButton(vr::k_EButton_Grip)) {
//     added_placeholder_grip = true;
      builder.AddPlaceholderButton();
//   }
  
  if (!builder.TryAddNextUnusedAxesButton())
//   bool added_placeholder_axes = false;
//   if (!builder.TryAddNextUnusedButtonWithAxes()) {
//     added_placeholder_axes = true;
      builder.AddPlaceholderButton();
//   }
  
//   bool added_optional_buttons = false;
    for (const auto& button : kWebXRButtonOrder) {
    builder.TryAddButton(button);
//     added_optional_buttons =
//         builder.TryAddButton(button) || added_optional_buttons;
    }
  
  builder.AddRemainingAxes();
//   bool added_optional_axes = builder.AddRemainingTriggersAndAxes();
// 
//    
//    
//   if (!(added_optional_buttons || added_optional_axes)) {
//      
//      
//      
//      
//      
//     if (added_placeholder_axes) {
//       builder.RemovePlaceholderButton();
// 
//        
//        
//       if (added_placeholder_grip) {
//         builder.RemovePlaceholderButton();
//       }
//     }
//   }
  
    return builder.GetGamepad();
  }
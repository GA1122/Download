GamepadBuilder::GamepadBuilder(const std::string& gamepad_id,
                               GamepadMapping mapping,
                               device::mojom::XRHandedness handedness)
    : mapping_(mapping) {
  DCHECK_LT(gamepad_id.size(), Gamepad::kIdLengthCap);

  auto mapping_str = GamepadMappingToString(mapping);
  DCHECK_LT(mapping_str.size(), Gamepad::kMappingLengthCap);

  gamepad_.connected = true;
  gamepad_.timestamp = base::TimeTicks::Now().since_origin().InMicroseconds();
  gamepad_.hand = MojoToGamepadHandedness(handedness);
  CopyToUString(base::UTF8ToUTF16(gamepad_id), gamepad_.id,
                Gamepad::kIdLengthCap);
  CopyToUString(base::UTF8ToUTF16(mapping_str), gamepad_.mapping,
                Gamepad::kMappingLengthCap);
}

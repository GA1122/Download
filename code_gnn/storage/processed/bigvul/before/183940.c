    bool ContainOnlyOneKeyboardLayout(
      const ImeConfigValue& value) {
    return (value.type == ImeConfigValue::kValueTypeStringList &&
//       const input_method::ImeConfigValue& value) {
//     return (value.type == input_method::ImeConfigValue::kValueTypeStringList &&
              value.string_list_value.size() == 1 &&
            chromeos::input_method::IsKeyboardLayout(
//             input_method::IsKeyboardLayout(
                  value.string_list_value[0]));
    }
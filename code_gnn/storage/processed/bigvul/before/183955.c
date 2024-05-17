   void MaybeChangeCurrentKeyboardLayout(const std::string& section,
                                        const std::string& config_name,
                                        const ImeConfigValue& value) {
//   void MaybeChangeCurrentKeyboardLayout(
//       const std::string& section,
//       const std::string& config_name,
//       const input_method::ImeConfigValue& value) {
  
     if (section == language_prefs::kGeneralSectionName &&
         config_name == language_prefs::kPreloadEnginesConfigName &&
         ContainOnlyOneKeyboardLayout(value)) {
       ChangeCurrentInputMethodFromId(value.string_list_value[0]);
     }
   }
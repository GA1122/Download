  virtual std::string GetKeyboardOverlayId(const std::string& input_method_id) {
    KeyboardOverlayMap::const_iterator iter =
        keyboard_overlay_map_->find(input_method_id);
    return (iter != keyboard_overlay_map_->end()) ?
         iter->second : "";
   }

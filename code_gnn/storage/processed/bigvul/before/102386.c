  virtual string16 Execute(
      const std::string& component_text,
      std::vector<size_t>* offsets_into_component) const {
    return (unescape_rules_ == UnescapeRule::NONE) ?
        UTF8ToUTF16AndAdjustOffsets(component_text, offsets_into_component) :
        UnescapeAndDecodeUTF8URLComponentWithOffsets(component_text,
            unescape_rules_, offsets_into_component);
  }

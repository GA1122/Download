  AccessorySheetDataBuilder& AppendField(const base::string16& display_text,
                                         const base::string16& a11y_description,
                                         bool is_obfuscated,
                                         bool selectable) {
    accessory_sheet_data_.mutable_user_info_list().back().add_field(
        UserInfo::Field(display_text, a11y_description, is_obfuscated,
                        selectable));
    return *this;
  }

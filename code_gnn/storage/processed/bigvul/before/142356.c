  AccessorySheetDataBuilder& AppendFooterCommand(
      const base::string16& display_text) {
    accessory_sheet_data_.add_footer_command(FooterCommand(display_text));
    return *this;
  }

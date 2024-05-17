AccessorySheetDataBuilder PasswordAccessorySheetDataBuilder(
    const base::string16& title) {
  AccessorySheetDataBuilder builder(title);
  builder.AppendFooterCommand(manage_passwords_str());
   return builder;
 }

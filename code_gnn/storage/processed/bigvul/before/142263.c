autofill::AccessorySheetData dummy_accessory_sheet_data() {
  constexpr char kExampleAccessorySheetDataTitle[] = "Example title";
  return autofill::AccessorySheetData(
      base::ASCIIToUTF16(kExampleAccessorySheetDataTitle));
}

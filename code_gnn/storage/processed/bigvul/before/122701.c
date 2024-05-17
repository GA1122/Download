bool Extension::LoadThemeColors(const DictionaryValue* theme_value,
                                string16* error) {
  const DictionaryValue* colors_value = NULL;
  if (theme_value->GetDictionary(keys::kThemeColors, &colors_value)) {
    for (DictionaryValue::Iterator iter(*colors_value); !iter.IsAtEnd();
         iter.Advance()) {
      const ListValue* color_list = NULL;
      double alpha = 0.0;
      int color = 0;
      if (!iter.value().GetAsList(&color_list) ||
          ((color_list->GetSize() != 3) &&
           ((color_list->GetSize() != 4) ||
            !color_list->GetDouble(3, &alpha))) ||
          !color_list->GetInteger(0, &color) ||
          !color_list->GetInteger(1, &color) ||
          !color_list->GetInteger(2, &color)) {
        *error = ASCIIToUTF16(errors::kInvalidThemeColors);
        return false;
      }
    }
    theme_colors_.reset(colors_value->DeepCopy());
  }
  return true;
}

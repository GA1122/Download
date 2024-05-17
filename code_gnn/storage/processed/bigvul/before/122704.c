bool Extension::LoadThemeTints(const DictionaryValue* theme_value,
                               string16* error) {
  const DictionaryValue* tints_value = NULL;
  if (!theme_value->GetDictionary(keys::kThemeTints, &tints_value))
    return true;

  for (DictionaryValue::Iterator iter(*tints_value); !iter.IsAtEnd();
       iter.Advance()) {
    const ListValue* tint_list = NULL;
    double v = 0.0;
    if (!iter.value().GetAsList(&tint_list) ||
        tint_list->GetSize() != 3 ||
        !tint_list->GetDouble(0, &v) ||
        !tint_list->GetDouble(1, &v) ||
        !tint_list->GetDouble(2, &v)) {
      *error = ASCIIToUTF16(errors::kInvalidThemeTints);
      return false;
    }
  }
  theme_tints_.reset(tints_value->DeepCopy());
  return true;
}

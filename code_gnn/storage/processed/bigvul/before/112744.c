int GetPageCountFromSettingsDictionary(const DictionaryValue& settings) {
  int count = 0;
  const ListValue* page_range_array;
  if (settings.GetList(printing::kSettingPageRange, &page_range_array)) {
    for (size_t index = 0; index < page_range_array->GetSize(); ++index) {
      const DictionaryValue* dict;
      if (!page_range_array->GetDictionary(index, &dict))
        continue;

      printing::PageRange range;
      if (!dict->GetInteger(printing::kSettingPageRangeFrom, &range.from) ||
          !dict->GetInteger(printing::kSettingPageRangeTo, &range.to)) {
        continue;
      }
      count += (range.to - range.from) + 1;
    }
  }
  return count;
}

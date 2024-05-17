Locale& Document::GetCachedLocale(const AtomicString& locale) {
  AtomicString locale_key = locale;
  if (locale.IsEmpty() ||
      !RuntimeEnabledFeatures::LangAttributeAwareFormControlUIEnabled())
    return Locale::DefaultLocale();
  LocaleIdentifierToLocaleMap::AddResult result =
      locale_cache_.insert(locale_key, nullptr);
  if (result.is_new_entry)
    result.stored_value->value = Locale::Create(locale_key);
  return *(result.stored_value->value);
}

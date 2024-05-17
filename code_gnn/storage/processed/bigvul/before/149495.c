void IDNSpoofChecker::SetAllowedUnicodeSet(UErrorCode* status) {
  if (U_FAILURE(*status))
    return;

  const icu::UnicodeSet* recommended_set =
      uspoof_getRecommendedUnicodeSet(status);
  icu::UnicodeSet allowed_set;
  allowed_set.addAll(*recommended_set);
  const icu::UnicodeSet* inclusion_set = uspoof_getInclusionUnicodeSet(status);
  allowed_set.addAll(*inclusion_set);

#if U_ICU_VERSION_MAJOR_NUM < 60
  const icu::UnicodeSet aspirational_scripts(
      icu::UnicodeString(
          "[\\u1401-\\u166C\\u166F-\\u167F"
          "\\u1810-\\u1819\\u1820-\\u1877\\u1880-\\u18AA"
          "\\u18B0-\\u18F5"
          "\\u2D30-\\u2D67\\u2D7F"
          "\\uA000-\\uA48C"
          "\\U00016F00-\\U00016F44\\U00016F50-\\U00016F7E"
          "\\U00016F8F-\\U00016F9F]",
          -1, US_INV),
      *status);
  allowed_set.addAll(aspirational_scripts);
#else
#error "Update aspirational_scripts per Unicode 10.0"
#endif

  allowed_set.remove(0x338u);
  allowed_set.remove(0x2010u);
  allowed_set.remove(0x2027u);

#if defined(OS_MACOSX)
  allowed_set.remove(0x0F8Cu);
  allowed_set.remove(0x0F8Du);
  allowed_set.remove(0x0F8Eu);
  allowed_set.remove(0x0F8Fu);
#endif

  uspoof_setAllowedUnicodeSet(checker_, &allowed_set, status);
}

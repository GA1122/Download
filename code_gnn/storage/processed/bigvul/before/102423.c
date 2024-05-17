bool IsIDNComponentSafe(const char16* str,
                        int str_len,
                        const std::string& languages) {

  UErrorCode status = U_ZERO_ERROR;
#ifdef U_WCHAR_IS_UTF16
  icu::UnicodeSet dangerous_characters(icu::UnicodeString(
      L"[[\\ \u00bc\u00bd\u01c3\u0337\u0338"
      L"\u05c3\u05f4\u06d4\u0702\u115f\u1160][\u2000-\u200b]"
      L"[\u2024\u2027\u2028\u2029\u2039\u203a\u2044\u205f]"
      L"[\u2154-\u2156][\u2159-\u215b][\u215f\u2215\u23ae"
      L"\u29f6\u29f8\u2afb\u2afd][\u2ff0-\u2ffb][\u3014"
      L"\u3015\u3033\u3164\u321d\u321e\u33ae\u33af\u33c6\u33df\ufe14"
      L"\ufe15\ufe3f\ufe5d\ufe5e\ufeff\uff0e\uff06\uff61\uffa0\ufff9]"
      L"[\ufffa-\ufffd]]"), status);
  DCHECK(U_SUCCESS(status));
  icu::RegexMatcher dangerous_patterns(icu::UnicodeString(
      L"[^\\p{Katakana}][\u30ce\u30f3\u30bd][^\\p{Katakana}]"
      L"|[\u3099\u309a\u309b\u309c][\u3099\u309a\u309b\u309c]"),
      0, status);
#else
  icu::UnicodeSet dangerous_characters(icu::UnicodeString(
      "[[\\u0020\\u00bc\\u00bd\\u01c3\\u0337\\u0338"
      "\\u05c3\\u05f4\\u06d4\\u0702\\u115f\\u1160][\\u2000-\\u200b]"
      "[\\u2024\\u2027\\u2028\\u2029\\u2039\\u203a\\u2044\\u205f]"
      "[\\u2154-\\u2156][\\u2159-\\u215b][\\u215f\\u2215\\u23ae"
      "\\u29f6\\u29f8\\u2afb\\u2afd][\\u2ff0-\\u2ffb][\\u3014"
      "\\u3015\\u3033\\u3164\\u321d\\u321e\\u33ae\\u33af\\u33c6\\u33df\\ufe14"
      "\\ufe15\\ufe3f\\ufe5d\\ufe5e\\ufeff\\uff0e\\uff06\\uff61\\uffa0\\ufff9]"
      "[\\ufffa-\\ufffd]]", -1, US_INV), status);
  DCHECK(U_SUCCESS(status));
  icu::RegexMatcher dangerous_patterns(icu::UnicodeString(
      "[^\\p{Katakana}][\\u30ce\\u30f3\u30bd][^\\p{Katakana}]"
      "|[\\u3099\\u309a\\u309b\\u309c][\\u3099\\u309a\\u309b\\u309c]"),
      0, status);
#endif
  DCHECK(U_SUCCESS(status));
  icu::UnicodeSet component_characters;
  icu::UnicodeString component_string(str, str_len);
  component_characters.addAll(component_string);
  if (dangerous_characters.containsSome(component_characters))
    return false;

  DCHECK(U_SUCCESS(status));
  dangerous_patterns.reset(component_string);
  if (dangerous_patterns.find())
    return false;

  if (languages.empty())
    return IsIDNComponentInSingleScript(str, str_len);

  icu::UnicodeSet common_characters(UNICODE_STRING_SIMPLE("[[0-9]\\-_+\\ ]"),
                                    status);
  DCHECK(U_SUCCESS(status));
  component_characters.removeAll(common_characters);

  StringTokenizer t(languages, ",");
  while (t.GetNext()) {
    if (IsComponentCoveredByLang(component_characters, t.token()))
      return true;
  }
  return false;
}

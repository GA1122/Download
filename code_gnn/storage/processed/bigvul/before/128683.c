base::string16 TemplateURL::AdjustedShortNameForLocaleDirection() const {
  base::string16 bidi_safe_short_name = data_.short_name();
  base::i18n::AdjustStringForLocaleDirection(&bidi_safe_short_name);
  return bidi_safe_short_name;
}

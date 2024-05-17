void OmniboxPopupViewGtk::SetupLayoutForMatch(
    PangoLayout* layout,
    const string16& text,
    const AutocompleteMatch::ACMatchClassifications& classifications,
    const GdkColor* base_color,
    const GdkColor* dim_color,
    const GdkColor* url_color,
    const std::string& prefix_text) {
  bool marked_with_lre = false;
  string16 localized_text = text;
  if (localized_text.length() > 2000)
    localized_text = localized_text.substr(0, 2000);
  bool is_rtl = base::i18n::IsRTL();
  if (is_rtl && !base::i18n::StringContainsStrongRTLChars(localized_text)) {
    localized_text.insert(0, 1, base::i18n::kLeftToRightEmbeddingMark);
    marked_with_lre = true;
  }

  size_t additional_offset = prefix_text.length();   
  std::string text_utf8 = prefix_text + UTF16ToUTF8(localized_text);

  PangoAttrList* attrs = pango_attr_list_new();

  PangoAttribute* base_fg_attr = pango_attr_foreground_new(
      base_color->red, base_color->green, base_color->blue);
  pango_attr_list_insert(attrs, base_fg_attr);   

  for (ACMatchClassifications::const_iterator i = classifications.begin();
       i != classifications.end(); ++i) {
    size_t offset = GetUTF8Offset(localized_text, i->offset) +
                    additional_offset;

    const GdkColor* color = base_color;
    if (i->style & ACMatchClassification::URL) {
      color = url_color;
      if (is_rtl && !marked_with_lre) {
        std::string lre(kLRE);
        text_utf8.insert(offset, lre);
        additional_offset += lre.length();
      }
    }

    if (i->style & ACMatchClassification::DIM)
      color = dim_color;

    PangoAttribute* fg_attr = pango_attr_foreground_new(
        color->red, color->green, color->blue);
    fg_attr->start_index = offset;
    pango_attr_list_insert(attrs, fg_attr);   

    PangoWeight weight = (i->style & ACMatchClassification::MATCH) ?
        PANGO_WEIGHT_BOLD : PANGO_WEIGHT_NORMAL;
    PangoAttribute* weight_attr = pango_attr_weight_new(weight);
    weight_attr->start_index = offset;
    pango_attr_list_insert(attrs, weight_attr);   
  }

  pango_layout_set_text(layout, text_utf8.data(), text_utf8.length());
  pango_layout_set_attributes(layout, attrs);   
  pango_attr_list_unref(attrs);
}

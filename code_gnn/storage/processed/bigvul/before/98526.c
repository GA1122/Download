void SetupLayoutForMatch(PangoLayout* layout,
    const std::wstring& text,
    AutocompleteMatch::ACMatchClassifications classifications,
    const GdkColor* base_color,
    const GdkColor* url_color,
    const std::string& prefix_text) {

  size_t additional_offset = prefix_text.size();   
  std::string text_utf8 = prefix_text + WideToUTF8(text);

  PangoAttrList* attrs = pango_attr_list_new();

  PangoAttribute* base_fg_attr = pango_attr_foreground_new(
      base_color->red, base_color->green, base_color->blue);
  pango_attr_list_insert(attrs, base_fg_attr);   

  for (ACMatchClassifications::const_iterator i = classifications.begin();
       i != classifications.end(); ++i) {
    size_t offset = GetUTF8Offset(text, i->offset) + additional_offset;

    const GdkColor* color = base_color;
    if (i->style & ACMatchClassification::URL) {
      color = url_color;
      std::string lre(kLRE);
      text_utf8.insert(offset, lre);
      additional_offset += lre.size();
    }

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

  pango_layout_set_text(layout, text_utf8.data(), text_utf8.size());
  pango_layout_set_attributes(layout, attrs);   
  pango_attr_list_unref(attrs);
}

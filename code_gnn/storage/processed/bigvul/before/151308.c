std::unique_ptr<TracedValue> InspectorParseAuthorStyleSheetEvent::Data(
    const CSSStyleSheetResource* cached_style_sheet) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("styleSheetUrl", cached_style_sheet->Url().GetString());
  return value;
}

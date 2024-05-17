StyleSheetList& Document::StyleSheets() {
  if (!style_sheet_list_)
    style_sheet_list_ = StyleSheetList::Create(this);
  return *style_sheet_list_;
}

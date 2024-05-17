void Document::setSelectedStylesheetSet(const String& a_string) {
  UseCounter::Count(*this, WebFeature::kDocumentSetSelectedStylesheetSet);
  GetStyleEngine().SetSelectedStylesheetSetName(a_string);
}

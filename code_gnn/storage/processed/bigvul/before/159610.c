String Document::selectedStylesheetSet() const {
  UseCounter::Count(*this, WebFeature::kDocumentGetSelectedStylesheetSet);
  return style_engine_->SelectedStylesheetSetName();
}

void PDFiumEngine::OnSelectionChanged() {
  pp::PDF::SetSelectedText(GetPluginInstance(), GetSelectedText().c_str());
}

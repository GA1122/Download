pp::VarArray PDFiumEngine::GetBookmarks() {
  pp::VarDictionary dict = TraverseBookmarks(doc_, nullptr, 0);
  return pp::VarArray(dict.Get(pp::Var("children")));
}

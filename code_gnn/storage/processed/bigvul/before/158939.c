pp::VarArray PDFiumEngine::GetBookmarks() {
  pp::VarDictionary dict = TraverseBookmarks(nullptr, 0);
  return pp::VarArray(dict.Get(pp::Var("children")));
}

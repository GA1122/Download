EditorClient& Editor::Client() const {
  if (Page* page = GetFrame().GetPage())
    return page->GetEditorClient();
  return GetEmptyEditorClient();
}

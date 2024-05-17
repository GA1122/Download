void Editor::clear() {
  frame().inputMethodController().clear();
  m_shouldStyleWithCSS = false;
  m_defaultParagraphSeparator = EditorParagraphSeparatorIsDiv;
  m_lastEditCommand = nullptr;
  m_undoStack->clear();
}

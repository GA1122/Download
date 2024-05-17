Editor::RevealSelectionScope::RevealSelectionScope(Editor* editor)
    : m_editor(editor) {
  ++m_editor->m_preventRevealSelection;
}

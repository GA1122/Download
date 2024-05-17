DEFINE_TRACE(Editor) {
  visitor->trace(m_frame);
  visitor->trace(m_lastEditCommand);
  visitor->trace(m_undoStack);
  visitor->trace(m_mark);
  visitor->trace(m_typingStyle);
}

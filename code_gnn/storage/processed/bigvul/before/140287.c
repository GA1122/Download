Editor::Editor(LocalFrame& frame)
    : m_frame(&frame),
      m_undoStack(UndoStack::create()),
      m_preventRevealSelection(0),
      m_shouldStartNewKillRingSequence(false),
      m_shouldStyleWithCSS(false),
      m_killRing(WTF::wrapUnique(new KillRing)),
      m_areMarkedTextMatchesHighlighted(false),
      m_defaultParagraphSeparator(EditorParagraphSeparatorIsDiv),
      m_overwriteModeEnabled(false) {}

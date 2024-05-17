Editor::Editor(LocalFrame& frame)
    : frame_(&frame),
      undo_stack_(UndoStack::Create()),
      prevent_reveal_selection_(0),
      should_start_new_kill_ring_sequence_(false),
      should_style_with_css_(false),
      kill_ring_(WTF::WrapUnique(new KillRing)),
      are_marked_text_matches_highlighted_(false),
      default_paragraph_separator_(kEditorParagraphSeparatorIsDiv),
      overwrite_mode_enabled_(false) {}

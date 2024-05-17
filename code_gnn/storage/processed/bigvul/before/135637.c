FrameSelection::FrameSelection(LocalFrame& frame)
    : frame_(frame),
      layout_selection_(LayoutSelection::Create(*this)),
      selection_editor_(SelectionEditor::Create(frame)),
      granularity_(TextGranularity::kCharacter),
      x_pos_for_vertical_arrow_navigation_(NoXPosForVerticalArrowNavigation()),
      focused_(frame.GetPage() &&
               frame.GetPage()->GetFocusController().FocusedFrame() == frame),
      frame_caret_(new FrameCaret(frame, *selection_editor_)) {}

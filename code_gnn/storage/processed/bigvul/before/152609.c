void TestRenderFrame::SetCompositionFromExistingText(
    int start,
    int end,
    const std::vector<ui::ImeTextSpan>& ime_text_spans) {
  GetFrameInputHandler()->SetCompositionFromExistingText(start, end,
                                                         ime_text_spans);
}

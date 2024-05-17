void RenderWidgetHostViewAura::SetCompositionText(
    const ui::CompositionText& composition) {
  if (!host_)
    return;

  COMPILE_ASSERT(sizeof(ui::CompositionUnderline) ==
                 sizeof(blink::WebCompositionUnderline),
                 ui_CompositionUnderline__WebKit_WebCompositionUnderline_diff);

  const std::vector<blink::WebCompositionUnderline>& underlines =
      reinterpret_cast<const std::vector<blink::WebCompositionUnderline>&>(
          composition.underlines);

  host_->ImeSetComposition(composition.text, underlines,
                           composition.selection.end(),
                           composition.selection.end());

  has_composition_text_ = !composition.text.empty();
}

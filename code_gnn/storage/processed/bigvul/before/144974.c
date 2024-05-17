void RenderWidgetHostViewAura::SetCompositionText(
    const ui::CompositionText& composition) {
  if (!text_input_manager_ || !text_input_manager_->GetActiveWidget())
    return;

  std::vector<blink::WebCompositionUnderline> underlines;
  underlines.reserve(composition.underlines.size());
  for (std::vector<ui::CompositionUnderline>::const_iterator it =
           composition.underlines.begin();
       it != composition.underlines.end(); ++it) {
    underlines.push_back(
        blink::WebCompositionUnderline(static_cast<unsigned>(it->start_offset),
                                       static_cast<unsigned>(it->end_offset),
                                       it->color,
                                       it->thick,
                                       it->background_color));
  }

  text_input_manager_->GetActiveWidget()->ImeSetComposition(
      composition.text, underlines, gfx::Range::InvalidRange(),
      composition.selection.end(), composition.selection.end());

  has_composition_text_ = !composition.text.empty();
}

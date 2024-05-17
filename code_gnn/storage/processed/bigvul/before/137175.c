void Textfield::ExtendSelectionAndDelete(size_t before, size_t after) {
  gfx::Range range = GetRenderText()->selection();
  DCHECK_GE(range.start(), before);

  range.set_start(range.start() - before);
  range.set_end(range.end() + after);
  gfx::Range text_range;
  if (GetTextRange(&text_range) && text_range.Contains(range))
    DeleteRange(range);
}

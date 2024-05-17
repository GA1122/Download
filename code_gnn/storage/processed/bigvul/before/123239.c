void RenderWidgetHostViewAura::SelectionChanged(const string16& text,
                                                size_t offset,
                                                const ui::Range& range) {
  RenderWidgetHostViewBase::SelectionChanged(text, offset, range);

#if defined(USE_X11) && !defined(OS_CHROMEOS)
  if (text.empty() || range.is_empty())
    return;

  ui::ScopedClipboardWriter clipboard_writer(
      ui::Clipboard::GetForCurrentThread(),
      ui::Clipboard::BUFFER_SELECTION);
  clipboard_writer.WriteText(text);
#endif   
}

void RenderWidgetHostViewAura::SelectionChanged(const base::string16& text,
                                                size_t offset,
                                                const gfx::Range& range) {
  RenderWidgetHostViewBase::SelectionChanged(text, offset, range);

#if defined(USE_X11) && !defined(OS_CHROMEOS)
  if (text.empty() || range.is_empty())
    return;

  ui::ScopedClipboardWriter clipboard_writer(
      ui::Clipboard::GetForCurrentThread(),
      ui::CLIPBOARD_TYPE_SELECTION);
  clipboard_writer.WriteText(text);
#endif   
}

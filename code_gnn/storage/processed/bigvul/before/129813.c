void SystemClipboard::WritePlainText(const String& plain_text,
                                     SmartReplaceOption) {
  String text = plain_text;
#if defined(OS_WIN)
  ReplaceNewlinesWithWindowsStyleNewlines(text);
#endif
  clipboard_->WriteText(mojom::ClipboardBuffer::kStandard, NonNullString(text));
  clipboard_->CommitWrite(mojom::ClipboardBuffer::kStandard);
}

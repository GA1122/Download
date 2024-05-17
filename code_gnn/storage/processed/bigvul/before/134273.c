void OmniboxViewViews::OnAfterCutOrCopy() {
  ui::Clipboard* cb = ui::Clipboard::GetForCurrentThread();
  base::string16 selected_text;
  cb->ReadText(ui::CLIPBOARD_TYPE_COPY_PASTE, &selected_text);
  GURL url;
  bool write_url;
  model()->AdjustTextForCopy(GetSelectedRange().GetMin(), IsSelectAll(),
                             &selected_text, &url, &write_url);
  if (IsSelectAll())
    UMA_HISTOGRAM_COUNTS(OmniboxEditModel::kCutOrCopyAllTextHistogram, 1);

  if (write_url) {
    BookmarkNodeData data;
    data.ReadFromTuple(url, selected_text);
    data.WriteToClipboard(ui::CLIPBOARD_TYPE_COPY_PASTE);
  } else {
    ui::ScopedClipboardWriter scoped_clipboard_writer(
        ui::Clipboard::GetForCurrentThread(), ui::CLIPBOARD_TYPE_COPY_PASTE);
    scoped_clipboard_writer.WriteText(selected_text);
  }
}

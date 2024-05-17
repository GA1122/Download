void OmniboxViewViews::OnAfterCutOrCopy(ui::ClipboardType clipboard_type) {
  ui::Clipboard* cb = ui::Clipboard::GetForCurrentThread();
  base::string16 selected_text;
  cb->ReadText(clipboard_type, &selected_text);
  GURL url;
  bool write_url = false;
  model()->AdjustTextForCopy(GetSelectedRange().GetMin(), &selected_text, &url,
                             &write_url);
  if (IsSelectAll())
    UMA_HISTOGRAM_COUNTS_1M(OmniboxEditModel::kCutOrCopyAllTextHistogram, 1);

  ui::ScopedClipboardWriter scoped_clipboard_writer(clipboard_type);
  scoped_clipboard_writer.WriteText(selected_text);

  if (write_url &&
      base::FeatureList::IsEnabled(kOmniboxCanCopyHyperlinksToClipboard)) {
    scoped_clipboard_writer.WriteHyperlink(selected_text, url.spec());
  }
}

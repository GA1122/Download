int OmniboxViewViews::OnDrop(const ui::OSExchangeData& data) {
  if (HasTextBeingDragged())
    return ui::DragDropTypes::DRAG_NONE;

  base::string16 text;
  if (data.HasURL(ui::OSExchangeData::CONVERT_FILENAMES)) {
    GURL url;
    base::string16 title;
    if (data.GetURLAndTitle(ui::OSExchangeData::CONVERT_FILENAMES, &url,
                            &title)) {
      text = StripJavascriptSchemas(base::UTF8ToUTF16(url.spec()));
    }
  } else if (data.HasString() && data.GetString(&text)) {
    text = StripJavascriptSchemas(base::CollapseWhitespace(text, true));
  } else {
    return ui::DragDropTypes::DRAG_NONE;
  }

  SetUserText(text);
  if (!HasFocus())
    RequestFocus();
  SelectAll(false);
  return ui::DragDropTypes::DRAG_COPY;
}

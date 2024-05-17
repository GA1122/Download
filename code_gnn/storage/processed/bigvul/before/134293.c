void OmniboxViewViews::OnWriteDragData(ui::OSExchangeData* data) {
  base::string16 selected_text = GetSelectedText();
  GURL url;
  bool write_url;
  bool is_all_selected = IsSelectAll();
  model()->AdjustTextForCopy(GetSelectedRange().GetMin(), is_all_selected,
                             &selected_text, &url, &write_url);
  data->SetString(selected_text);
  if (write_url) {
    gfx::Image favicon;
    base::string16 title = selected_text;
    if (is_all_selected)
      model()->GetDataForURLExport(&url, &title, &favicon);
    button_drag_utils::SetURLAndDragImage(url, title, favicon.AsImageSkia(),
                                          data, GetWidget());
    data->SetURL(url, title);
  }
}

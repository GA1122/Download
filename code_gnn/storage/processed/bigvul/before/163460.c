void OmniboxViewViews::UpdateContextMenu(ui::SimpleMenuModel* menu_contents) {
  int paste_position = menu_contents->GetIndexOfCommandId(IDS_APP_PASTE);
  DCHECK_GE(paste_position, 0);
  menu_contents->InsertItemWithStringIdAt(
      paste_position + 1, IDS_PASTE_AND_GO, IDS_PASTE_AND_GO);

  menu_contents->AddSeparator(ui::NORMAL_SEPARATOR);

  menu_contents->AddItemWithStringId(IDC_EDIT_SEARCH_ENGINES,
      IDS_EDIT_SEARCH_ENGINES);
}

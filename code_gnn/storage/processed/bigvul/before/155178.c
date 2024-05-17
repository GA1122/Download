void OmniboxViewViews::UpdateContextMenu(ui::SimpleMenuModel* menu_contents) {
  if (send_tab_to_self::ShouldOfferFeature(
          location_bar_view_->GetWebContents())) {
    send_tab_to_self::RecordSendTabToSelfClickResult(
        send_tab_to_self::kOmniboxMenu, SendTabToSelfClickResult::kShowItem);
    int index = menu_contents->GetIndexOfCommandId(IDS_APP_UNDO);
    if (index)
      menu_contents->InsertSeparatorAt(index++, ui::NORMAL_SEPARATOR);
    menu_contents->InsertItemWithStringIdAt(index, IDC_SEND_TAB_TO_SELF,
                                            IDS_CONTEXT_MENU_SEND_TAB_TO_SELF);
    menu_contents->SetIcon(index,
                           gfx::Image(*send_tab_to_self::GetImageSkia()));
    menu_contents->InsertSeparatorAt(++index, ui::NORMAL_SEPARATOR);
  }

  int paste_position = menu_contents->GetIndexOfCommandId(IDS_APP_PASTE);
  DCHECK_GE(paste_position, 0);
  menu_contents->InsertItemWithStringIdAt(paste_position + 1, IDC_PASTE_AND_GO,
                                          IDS_PASTE_AND_GO);

  if (base::FeatureList::IsEnabled(omnibox::kQueryInOmnibox)) {
    if (!read_only() && !model()->user_input_in_progress() &&
        text() != controller()->GetLocationBarModel()->GetFormattedFullURL()) {
      menu_contents->AddItemWithStringId(IDS_SHOW_URL, IDS_SHOW_URL);
    }
  }

  menu_contents->AddSeparator(ui::NORMAL_SEPARATOR);

  menu_contents->AddItemWithStringId(IDC_EDIT_SEARCH_ENGINES,
                                     IDS_EDIT_SEARCH_ENGINES);
}

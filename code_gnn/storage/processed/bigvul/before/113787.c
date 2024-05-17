void PrintPreviewUI::SetSourceIsModifiable(
    TabContentsWrapper* print_preview_tab,
    bool source_is_modifiable) {
  if (!print_preview_tab || !print_preview_tab->web_contents()->GetWebUI())
    return;
  PrintPreviewUI* print_preview_ui = static_cast<PrintPreviewUI*>(
      print_preview_tab->web_contents()->GetWebUI()->GetController());
  print_preview_ui->source_is_modifiable_ = source_is_modifiable;
}

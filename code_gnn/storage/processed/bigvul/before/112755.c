void PrintPreviewHandler::HandlePrint(const ListValue* args) {
  ReportStats();

  UMA_HISTOGRAM_COUNTS("PrintPreview.RegeneratePreviewRequest.BeforePrint",
                       regenerate_preview_request_count_);

  TabContents* initiator_tab = GetInitiatorTab();
  if (initiator_tab) {
    RenderViewHost* rvh = initiator_tab->web_contents()->GetRenderViewHost();
    rvh->Send(new PrintMsg_ResetScriptedPrintCount(rvh->GetRoutingID()));
  }

  scoped_ptr<DictionaryValue> settings(GetSettingsDictionary(args));
  if (!settings.get())
    return;

  printing::StickySettings* sticky_settings = GetStickySettings();
  sticky_settings->Store(*settings);
  sticky_settings->SaveInPrefs(Profile::FromBrowserContext(
      preview_web_contents()->GetBrowserContext())->GetPrefs());
  settings->SetBoolean(printing::kSettingHeaderFooterEnabled, false);

  bool print_to_pdf = false;
  bool is_cloud_printer = false;
  bool is_cloud_dialog = false;

  bool open_pdf_in_preview = false;
#if defined(OS_MACOSX)
  open_pdf_in_preview = settings->HasKey(printing::kSettingOpenPDFInPreview);
#endif

  if (!open_pdf_in_preview) {
    settings->GetBoolean(printing::kSettingPrintToPDF, &print_to_pdf);
    settings->GetBoolean(printing::kSettingCloudPrintDialog, &is_cloud_dialog);
    is_cloud_printer = settings->HasKey(printing::kSettingCloudPrintId);
  }

  if (is_cloud_printer) {
    std::string print_ticket;
    bool res = args->GetString(1, &print_ticket);
    DCHECK(res);
    SendCloudPrintJob(*settings, print_ticket);
  } else if (print_to_pdf) {
    HandlePrintToPdf(*settings);
  } else if (is_cloud_dialog) {
    HandlePrintWithCloudPrint(NULL);
  } else {
    ReportPrintSettingsStats(*settings);
    ReportUserActionHistogram(PRINT_TO_PRINTER);
    UMA_HISTOGRAM_COUNTS("PrintPreview.PageCount.PrintToPrinter",
                         GetPageCountFromSettingsDictionary(*settings));

    PrintPreviewUI* print_preview_ui = static_cast<PrintPreviewUI*>(
        web_ui()->GetController());
    print_preview_ui->OnHidePreviewTab();

    ClearInitiatorTabDetails();

    settings->Remove(printing::kSettingPageRange, NULL);
    RenderViewHost* rvh = preview_web_contents()->GetRenderViewHost();
    rvh->Send(
        new PrintMsg_PrintForPrintPreview(rvh->GetRoutingID(), *settings));

    if (initiator_tab)
      initiator_tab->print_view_manager()->PrintPreviewDone();
  }
}

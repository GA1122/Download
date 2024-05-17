void PrintPreviewHandler::SendCloudPrintJob(const DictionaryValue& settings,
                                            std::string print_ticket) {
  ReportUserActionHistogram(PRINT_WITH_CLOUD_PRINT);
  scoped_refptr<base::RefCountedBytes> data;
  PrintPreviewUI* print_preview_ui = static_cast<PrintPreviewUI*>(
      web_ui()->GetController());
  print_preview_ui->GetPrintPreviewDataForIndex(
      printing::COMPLETE_PREVIEW_DOCUMENT_INDEX, &data);
  if (data.get() && data->size() > 0U && data->front()) {
    string16 print_job_title_utf16 =
        preview_tab_contents()->print_view_manager()->RenderSourceName();
    std::string print_job_title = UTF16ToUTF8(print_job_title_utf16);
    std::string printer_id;
    settings.GetString(printing::kSettingCloudPrintId, &printer_id);
    std::string raw_data(reinterpret_cast<const char*>(data->front()),
                         data->size());
    std::string base64_data;
    if (!base::Base64Encode(raw_data, &base64_data)) {
      NOTREACHED() << "Base64 encoding PDF data.";
    }

    const char boundary[] = "----CloudPrintFormBoundaryjc9wuprokl8i";
    const char prolog[] = "--%s\r\n"
      "Content-Disposition: form-data; name=\"capabilities\"\r\n\r\n%s\r\n"
      "--%s\r\n"
      "Content-Disposition: form-data; name=\"contentType\"\r\n\r\ndataUrl\r\n"
      "--%s\r\n"
      "Content-Disposition: form-data; name=\"title\"\r\n\r\n%s\r\n"
      "--%s\r\n"
      "Content-Disposition: form-data; name=\"printerid\"\r\n\r\n%s\r\n"
      "--%s\r\n"
      "Content-Disposition: form-data; name=\"content\"\r\n\r\n"
      "data:application/pdf;base64,%s\r\n"
      "--%s\r\n";

    std::string final_data;
    base::SStringPrintf(&final_data,
                        prolog,
                        boundary,
                        print_ticket.c_str(),
                        boundary,
                        boundary,
                        print_job_title.c_str(),
                        boundary,
                        printer_id.c_str(),
                        boundary,
                        base64_data.c_str(),
                        boundary);

    StringValue data_value(final_data);

    web_ui()->CallJavascriptFunction("printToCloud", data_value);
  } else {
    NOTREACHED();
  }
}

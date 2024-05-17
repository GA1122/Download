void PrintPreviewMessageHandler::OnMetafileReadyForPrinting(
    const PrintHostMsg_DidPreviewDocument_Params& params) {
  StopWorker(params.document_cookie);

  if (params.expected_pages_count <= 0) {
    NOTREACHED();
    return;
  }

  PrintPreviewUI* print_preview_ui = GetPrintPreviewUI();
  if (!print_preview_ui)
    return;

  if (params.reuse_existing_data) {
    PrintHostMsg_DidGetPreviewPageCount_Params temp_params;
    temp_params.page_count = params.expected_pages_count;
    temp_params.document_cookie = params.document_cookie;
    temp_params.is_modifiable = params.modifiable;
    temp_params.preview_request_id = params.preview_request_id;
    print_preview_ui->OnDidGetPreviewPageCount(temp_params);
    print_preview_ui->OnReusePreviewData(params.preview_request_id);
    return;
  }

  base::RefCountedBytes* data_bytes =
      GetDataFromHandle(params.metafile_data_handle, params.data_size);
  if (!data_bytes)
    return;

  print_preview_ui->SetPrintPreviewDataForIndex(COMPLETE_PREVIEW_DOCUMENT_INDEX,
                                                data_bytes);
  print_preview_ui->OnPreviewDataIsAvailable(
      params.expected_pages_count, params.preview_request_id);
}

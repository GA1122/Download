 void PrintPreviewMessageHandler::OnDidPreviewPage(
     const PrintHostMsg_DidPreviewPage_Params& params) {
   int page_number = params.page_number;
   if (page_number < FIRST_PAGE_INDEX || !params.data_size)
     return;
 
   PrintPreviewUI* print_preview_ui = GetPrintPreviewUI();
    if (!print_preview_ui)
      return;
  
  scoped_refptr<base::RefCountedBytes> data_bytes =
      GetDataFromHandle(params.metafile_data_handle, params.data_size);
  DCHECK(data_bytes);
  print_preview_ui->SetPrintPreviewDataForIndex(page_number,
                                                std::move(data_bytes));
  print_preview_ui->OnDidPreviewPage(page_number, params.preview_request_id);
//   if (IsOopifEnabled() && print_preview_ui->source_is_modifiable()) {
//     auto* client = PrintCompositeClient::FromWebContents(web_contents());
//     DCHECK(client);
// 
//      
//     client->DoComposite(
//         params.metafile_data_handle, params.data_size,
//         base::BindOnce(&PrintPreviewMessageHandler::OnCompositePdfPageDone,
//                        weak_ptr_factory_.GetWeakPtr(), params.page_number,
//                        params.preview_request_id));
//   } else {
//     NotifyUIPreviewPageReady(
//         page_number, params.preview_request_id,
//         GetDataFromHandle(params.metafile_data_handle, params.data_size));
//   }
  }
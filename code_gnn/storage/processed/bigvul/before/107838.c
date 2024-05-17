void PrintWebViewHelper::CreatePreviewDocument(
    const ViewMsg_PrintPages_Params& params,
    WebFrame* frame,
    ViewHostMsg_DidPreviewDocument_Params* print_params) {
  print_params->document_cookie = params.params.document_cookie;
}

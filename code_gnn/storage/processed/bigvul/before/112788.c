void PrintPreviewUI::OnDidGetPreviewPageCount(
    const PrintHostMsg_DidGetPreviewPageCount_Params& params) {
  DCHECK_GT(params.page_count, 0);
  base::FundamentalValue count(params.page_count);
  base::FundamentalValue request_id(params.preview_request_id);
  web_ui()->CallJavascriptFunction("onDidGetPreviewPageCount",
                                   count,
                                   request_id);
}

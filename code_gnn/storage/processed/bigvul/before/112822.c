bool PrintWebViewHelper::CreatePreviewDocument() {
  PrintMsg_Print_Params print_params = print_pages_params_->params;
  const std::vector<int>& pages = print_pages_params_->pages;
  if (!print_preview_context_.CreatePreviewDocument(&print_params, pages,
                                                    ignore_css_margins_)) {
    return false;
  }

  PageSizeMargins default_page_layout;
  ComputePageLayoutInPointsForCss(print_preview_context_.frame(), 0,
                                  print_params, ignore_css_margins_, NULL,
                                  &default_page_layout);

  if (!old_print_pages_params_.get() ||
      !PageLayoutIsEqual(*old_print_pages_params_, *print_pages_params_)) {
    bool has_page_size_style = PrintingFrameHasPageSizeStyle(
        print_preview_context_.frame(),
        print_preview_context_.total_page_count());
    int dpi = GetDPI(&print_params);
    gfx::Rect printable_area_in_points(
      ConvertUnit(print_pages_params_->params.printable_area.x(),
          dpi, printing::kPointsPerInch),
      ConvertUnit(print_pages_params_->params.printable_area.y(),
          dpi, printing::kPointsPerInch),
      ConvertUnit(print_pages_params_->params.printable_area.width(),
          dpi, printing::kPointsPerInch),
      ConvertUnit(print_pages_params_->params.printable_area.height(),
          dpi, printing::kPointsPerInch));

    Send(new PrintHostMsg_DidGetDefaultPageLayout(routing_id(),
                                                  default_page_layout,
                                                  printable_area_in_points,
                                                  has_page_size_style));
  }

  PrintHostMsg_DidGetPreviewPageCount_Params params;
  params.page_count = print_preview_context_.total_page_count();
  params.is_modifiable = print_preview_context_.IsModifiable();
  params.document_cookie = print_pages_params_->params.document_cookie;
  params.preview_request_id = print_pages_params_->params.preview_request_id;
  params.clear_preview_data = print_preview_context_.generate_draft_pages();
  Send(new PrintHostMsg_DidGetPreviewPageCount(routing_id(), params));
  if (CheckForCancel())
    return false;

  while (!print_preview_context_.IsFinalPageRendered()) {
    int page_number = print_preview_context_.GetNextPageNumber();
    DCHECK_GE(page_number, 0);
    if (!RenderPreviewPage(page_number))
      return false;

    if (CheckForCancel())
      return false;

    if (print_preview_context_.IsFinalPageRendered())
      print_preview_context_.AllPagesRendered();

    if (print_preview_context_.IsLastPageOfPrintReadyMetafile()) {
      DCHECK(print_preview_context_.IsModifiable() ||
             print_preview_context_.IsFinalPageRendered());
      if (!FinalizePrintReadyDocument())
        return false;
    }
  }
  print_preview_context_.Finished();
  return true;
}

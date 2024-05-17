bool PrintRenderFrameHelper::CreatePreviewDocument() {
  if (!print_pages_params_ || CheckForCancel())
    return false;

  UMA_HISTOGRAM_ENUMERATION("PrintPreview.PreviewEvent",
                            PREVIEW_EVENT_CREATE_DOCUMENT, PREVIEW_EVENT_MAX);

  const PrintMsg_Print_Params& print_params = print_pages_params_->params;
  const std::vector<int>& pages = print_pages_params_->pages;

  if (!print_preview_context_.CreatePreviewDocument(
          std::move(prep_frame_view_), pages, print_params.printed_doc_type)) {
    return false;
  }

  PageSizeMargins default_page_layout;
  double scale_factor =
      print_params.scale_factor >= kEpsilon ? print_params.scale_factor : 1.0f;

  ComputePageLayoutInPointsForCss(print_preview_context_.prepared_frame(), 0,
                                  print_params, ignore_css_margins_,
                                  &scale_factor, &default_page_layout);
  bool has_page_size_style =
      PrintingFrameHasPageSizeStyle(print_preview_context_.prepared_frame(),
                                    print_preview_context_.total_page_count());
  int dpi = GetDPI(&print_params);

  gfx::Rect printable_area_in_points(
      ConvertUnit(print_params.printable_area.x(), dpi, kPointsPerInch),
      ConvertUnit(print_params.printable_area.y(), dpi, kPointsPerInch),
      ConvertUnit(print_params.printable_area.width(), dpi, kPointsPerInch),
      ConvertUnit(print_params.printable_area.height(), dpi, kPointsPerInch));

  double fit_to_page_scale_factor = 1.0f;
  if (!print_preview_context_.IsModifiable()) {
    blink::WebLocalFrame* source_frame = print_preview_context_.source_frame();
    const blink::WebNode& source_node = print_preview_context_.source_node();
    blink::WebPrintPresetOptions preset_options;
    if (source_frame->GetPrintPresetOptionsForPlugin(source_node,
                                                     &preset_options)) {
      if (preset_options.is_page_size_uniform) {
        bool is_printable_area_landscape = printable_area_in_points.width() >
                                           printable_area_in_points.height();
        bool is_preset_landscape = preset_options.uniform_page_size.width >
                                   preset_options.uniform_page_size.height;
        bool rotate = is_printable_area_landscape != is_preset_landscape;
        double printable_width = rotate ? printable_area_in_points.height()
                                        : printable_area_in_points.width();
        double printable_height = rotate ? printable_area_in_points.width()
                                         : printable_area_in_points.height();
        double scale_width =
            printable_width /
            static_cast<double>(preset_options.uniform_page_size.width);
        double scale_height =
            printable_height /
            static_cast<double>(preset_options.uniform_page_size.height);
        fit_to_page_scale_factor = std::min(scale_width, scale_height);
      } else {
        fit_to_page_scale_factor = 0.0f;
      }
    }
  }
  int fit_to_page_scaling = static_cast<int>(100.0f * fit_to_page_scale_factor);
  Send(new PrintHostMsg_DidGetDefaultPageLayout(
      routing_id(), default_page_layout, printable_area_in_points,
      has_page_size_style));

  PrintHostMsg_DidGetPreviewPageCount_Params params;
  params.page_count = print_preview_context_.total_page_count();
  params.fit_to_page_scaling = fit_to_page_scaling;
  params.preview_request_id = print_params.preview_request_id;
  params.clear_preview_data = print_preview_context_.generate_draft_pages() ||
                              !print_preview_context_.IsModifiable();
  Send(new PrintHostMsg_DidGetPreviewPageCount(routing_id(), params));
  if (CheckForCancel())
    return false;

  while (!print_preview_context_.IsFinalPageRendered()) {
    int page_number = print_preview_context_.GetNextPageNumber();
    DCHECK_GE(page_number, 0);
    if (!RenderPreviewPage(page_number, print_params))
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

void PrintRenderFrameHelper::PrintPageInternal(
    const PrintMsg_Print_Params& params,
    int page_number,
    int page_count,
    blink::WebLocalFrame* frame,
    PdfMetafileSkia* metafile,
    gfx::Size* page_size_in_dpi,
    gfx::Rect* content_area_in_dpi) {
  double css_scale_factor =
      params.scale_factor >= kEpsilon ? params.scale_factor : 1.0f;

  gfx::Size original_page_size = params.page_size;
  PageSizeMargins page_layout_in_points;
  ComputePageLayoutInPointsForCss(frame, page_number, params,
                                  ignore_css_margins_, &css_scale_factor,
                                  &page_layout_in_points);

  gfx::Size page_size;
  gfx::Rect content_area;
  GetPageSizeAndContentAreaFromPageLayout(page_layout_in_points, &page_size,
                                          &content_area);

  if (page_size_in_dpi)
    *page_size_in_dpi = original_page_size;

  if (content_area_in_dpi) {
    *content_area_in_dpi =
        gfx::Rect(0, 0, page_size_in_dpi->width(), page_size_in_dpi->height());
  }

  gfx::Rect canvas_area =
      params.display_header_footer ? gfx::Rect(page_size) : content_area;

#if defined(OS_WIN)
  float webkit_page_shrink_factor = frame->GetPrintPageShrink(page_number);
  float scale_factor = css_scale_factor * webkit_page_shrink_factor;
#else
  float scale_factor = css_scale_factor;
#endif

  cc::PaintCanvas* canvas =
      metafile->GetVectorCanvasForNewPage(page_size, canvas_area, scale_factor);
  if (!canvas)
    return;

  MetafileSkiaWrapper::SetMetafileOnCanvas(canvas, metafile);

  if (params.display_header_footer) {
#if defined(OS_WIN)
    const float fudge_factor = 1;
#else
    const float fudge_factor = kPrintingMinimumShrinkFactor;
#endif
    PrintHeaderAndFooter(canvas, page_number + 1, page_count, *frame,
                         scale_factor / fudge_factor, page_layout_in_points,
                         params);
  }

  float webkit_scale_factor = RenderPageContent(
      frame, page_number, canvas_area, content_area, scale_factor, canvas);
  DCHECK_GT(webkit_scale_factor, 0.0f);

  bool ret = metafile->FinishPage();
  DCHECK(ret);
 }

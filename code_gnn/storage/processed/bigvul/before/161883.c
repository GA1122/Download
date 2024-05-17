PrintMsg_Print_Params CalculatePrintParamsForCss(
    blink::WebLocalFrame* frame,
    int page_index,
    const PrintMsg_Print_Params& page_params,
    bool ignore_css_margins,
    bool fit_to_page,
    double* scale_factor) {
  PrintMsg_Print_Params css_params =
      GetCssPrintParams(frame, page_index, page_params);

  PrintMsg_Print_Params params = page_params;
  EnsureOrientationMatches(css_params, &params);

  params.content_size = ScaleAndRoundSize(params.content_size, *scale_factor);
  if (ignore_css_margins && fit_to_page)
    return params;

  PrintMsg_Print_Params result_params = css_params;
  bool scale = !params.print_to_pdf;
  double page_scaling = scale ? *scale_factor : 1.0f;
  if (!fit_to_page) {
    result_params.page_size =
        ScaleAndRoundSize(result_params.page_size, page_scaling);
  }
  if (ignore_css_margins) {
    params.margin_left = ScaleAndRound(params.margin_left, page_scaling);
    params.margin_top = ScaleAndRound(params.margin_top, page_scaling);
    params.page_size = ScaleAndRoundSize(params.page_size, page_scaling);

    result_params.margin_top = params.margin_top;
    result_params.margin_left = params.margin_left;

    DCHECK(!fit_to_page);
    int default_margin_right = params.page_size.width() -
                               params.content_size.width() - params.margin_left;
    int default_margin_bottom = params.page_size.height() -
                                params.content_size.height() -
                                params.margin_top;
    result_params.content_size =
        gfx::Size(result_params.page_size.width() - result_params.margin_left -
                      default_margin_right,
                  result_params.page_size.height() - result_params.margin_top -
                      default_margin_bottom);
  } else {
    result_params.content_size =
        ScaleAndRoundSize(result_params.content_size, *scale_factor);
    if (fit_to_page) {
      double factor = FitPrintParamsToPage(params, &result_params);
      if (scale_factor)
        *scale_factor *= factor;
    } else {
      result_params.margin_left =
          ScaleAndRound(result_params.margin_left, page_scaling);
      result_params.margin_top =
          ScaleAndRound(result_params.margin_top, page_scaling);
    }
  }

  return result_params;
}

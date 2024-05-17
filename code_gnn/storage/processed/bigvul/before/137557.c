void PrintWebViewHelper::PrintHeaderAndFooter(
    blink::WebCanvas* canvas,
    int page_number,
    int total_pages,
    const blink::WebFrame& source_frame,
    float webkit_scale_factor,
    const PageSizeMargins& page_layout,
    const PrintMsg_Print_Params& params) {
  SkAutoCanvasRestore auto_restore(canvas, true);
  canvas->scale(1 / webkit_scale_factor, 1 / webkit_scale_factor);

  blink::WebSize page_size(page_layout.margin_left + page_layout.margin_right +
                               page_layout.content_width,
                           page_layout.margin_top + page_layout.margin_bottom +
                               page_layout.content_height);

  blink::WebView* web_view = blink::WebView::create(NULL);
  web_view->settings()->setJavaScriptEnabled(true);

  blink::WebLocalFrame* frame =
      blink::WebLocalFrame::create(blink::WebTreeScopeType::Document, NULL);
  web_view->setMainFrame(frame);

  base::StringValue html(ResourceBundle::GetSharedInstance().GetLocalizedString(
      IDR_PRINT_PREVIEW_PAGE));
  ExecuteScript(frame, kPageLoadScriptFormat, html);

  scoped_ptr<base::DictionaryValue> options(new base::DictionaryValue());
  options.reset(new base::DictionaryValue());
  options->SetDouble(kSettingHeaderFooterDate, base::Time::Now().ToJsTime());
  options->SetDouble("width", page_size.width);
  options->SetDouble("height", page_size.height);
  options->SetDouble("topMargin", page_layout.margin_top);
  options->SetDouble("bottomMargin", page_layout.margin_bottom);
  options->SetString("pageNumber",
                     base::StringPrintf("%d/%d", page_number, total_pages));

  base::string16 url = source_frame.document().url().string();
  options->SetString("url", url.empty() ? params.url : url);
  base::string16 title = source_frame.document().title();
  options->SetString("title", title.empty() ? params.title : title);

  ExecuteScript(frame, kPageSetupScriptFormat, *options);

  blink::WebPrintParams webkit_params(page_size);
  webkit_params.printerDPI = GetDPI(&params);

  frame->printBegin(webkit_params);
  frame->printPage(0, canvas);
  frame->printEnd();

  web_view->close();
  frame->close();
}

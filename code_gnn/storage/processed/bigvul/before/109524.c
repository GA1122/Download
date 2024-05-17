void PrintWebViewHelper::OnPrintForPrintPreview(
    const DictionaryValue& job_settings) {
  DCHECK(is_preview_enabled_);
  if (print_web_view_)
    return;

  if (!render_view()->GetWebView())
    return;
  WebKit::WebFrame* main_frame = render_view()->GetWebView()->mainFrame();
  if (!main_frame)
    return;

  WebKit::WebDocument document = main_frame->document();
  WebKit::WebElement pdf_element = document.getElementById("pdf-viewer");
  if (pdf_element.isNull()) {
    NOTREACHED();
    return;
  }

  AutoReset<bool> set_printing_flag(&print_for_preview_, true);

  WebKit::WebFrame* pdf_frame = pdf_element.document().frame();
  if (!UpdatePrintSettings(pdf_frame, pdf_element, job_settings)) {
    LOG(ERROR) << "UpdatePrintSettings failed";
    DidFinishPrinting(FAIL_PRINT);
    return;
  }

  PrintMsg_Print_Params& print_params = print_pages_params_->params;
  print_params.printable_area = gfx::Rect(print_params.page_size);

  if (!RenderPagesForPrint(pdf_frame, pdf_element)) {
    LOG(ERROR) << "RenderPagesForPrint failed";
    DidFinishPrinting(FAIL_PRINT);
  }
}

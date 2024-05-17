void PrintWebViewHelper::OnPrintForPrintPreview(
    const DictionaryValue& job_settings) {
  DCHECK(is_preview_enabled_);
  if (print_web_view_)
    return;

  if (!render_view()->GetWebView())
    return;
  WebFrame* main_frame = render_view()->GetWebView()->mainFrame();
  if (!main_frame)
    return;

  WebDocument document = main_frame->document();
  WebElement pdf_element = document.getElementById("pdf-viewer");
  if (pdf_element.isNull()) {
    NOTREACHED();
    return;
  }

  AutoReset<bool> set_printing_flag(&print_for_preview_, true);

  WebFrame* pdf_frame = pdf_element.document().frame();
  if (!UpdatePrintSettings(pdf_frame, pdf_element, job_settings)) {
    LOG(ERROR) << "UpdatePrintSettings failed";
    DidFinishPrinting(FAIL_PRINT);
    return;
  }

  if (!RenderPagesForPrint(pdf_frame, pdf_element)) {
    LOG(ERROR) << "RenderPagesForPrint failed";
    DidFinishPrinting(FAIL_PRINT);
  }
}

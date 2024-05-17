void PrintRenderFrameHelper::OnPrintForPrintPreview(
    const base::DictionaryValue& job_settings) {
  CHECK_LE(ipc_nesting_level_, 1);
  if (prep_frame_view_)
    return;

  blink::WebDocument document = render_frame()->GetWebFrame()->GetDocument();
  blink::WebElement pdf_element = document.GetElementById("pdf-viewer");
  if (pdf_element.IsNull()) {
    NOTREACHED();
    return;
  }

  blink::WebLocalFrame* plugin_frame = pdf_element.GetDocument().GetFrame();
  blink::WebElement plugin_element = pdf_element;
  if (pdf_element.HasHTMLTagName("iframe")) {
    plugin_frame = blink::WebLocalFrame::FromFrameOwnerElement(pdf_element);
    plugin_element = delegate_->GetPdfElement(plugin_frame);
    if (plugin_element.IsNull()) {
      NOTREACHED();
      return;
    }
  }

  base::AutoReset<bool> set_printing_flag(&print_for_preview_, true);

  if (!UpdatePrintSettings(plugin_frame, plugin_element, job_settings)) {
    LOG(ERROR) << "UpdatePrintSettings failed";
    DidFinishPrinting(FAIL_PRINT);
    return;
  }

  PrintMsg_Print_Params& print_params = print_pages_params_->params;
  printer_printable_area_ = print_params.printable_area;
  print_params.printable_area = gfx::Rect(print_params.page_size);

  if (!RenderPagesForPrint(plugin_frame, plugin_element)) {
    LOG(ERROR) << "RenderPagesForPrint failed";
    DidFinishPrinting(FAIL_PRINT);
  }
}

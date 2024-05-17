void PrintingMessageFilter::GetPrintSettingsForRenderView(
    int render_view_id,
    GetPrintSettingsForRenderViewParams params,
    const base::Closure& callback,
    scoped_refptr<printing::PrinterQuery> printer_query) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  content::WebContents* wc = GetWebContentsForRenderView(render_view_id);

  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::Bind(&printing::PrinterQuery::GetSettings, printer_query,
                 params.ask_user_for_settings, wc->GetView()->GetNativeView(),
                 params.expected_page_count, params.has_selection,
                 params.margin_type, callback));
}

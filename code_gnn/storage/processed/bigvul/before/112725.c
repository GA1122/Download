void PrintingMessageFilter::CreatePrintDialogForFile(int render_view_id,
                                                     const FilePath& path) {
  content::WebContents* wc = GetWebContentsForRenderView(render_view_id);
  print_dialog_cloud::CreatePrintDialogForFile(
      wc->GetBrowserContext(),
      wc->GetView()->GetTopLevelNativeWindow(),
      path,
      string16(),
      string16(),
      std::string("application/pdf"),
      false);
}

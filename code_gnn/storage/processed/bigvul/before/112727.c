content::WebContents* PrintingMessageFilter::GetWebContentsForRenderView(
    int render_view_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  content::RenderViewHost* view = content::RenderViewHost::FromID(
      render_process_id_, render_view_id);
  return content::WebContents::FromRenderViewHost(view);
}

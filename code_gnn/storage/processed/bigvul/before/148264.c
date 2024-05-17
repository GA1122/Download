content::WebContents* PrintingContextDelegate::GetWebContents() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  auto* rfh =
      content::RenderFrameHost::FromID(render_process_id_, render_frame_id_);
  return rfh ? content::WebContents::FromRenderFrameHost(rfh) : nullptr;
}

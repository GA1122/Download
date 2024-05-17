TabSpecificContentSettings* TabSpecificContentSettings::Get(
    int render_process_id, int render_view_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  RenderViewHost* view = RenderViewHost::FromID(render_process_id,
                                                render_view_id);
  if (!view)
    return NULL;

  WebContents* web_contents = WebContents::FromRenderViewHost(view);
  if (!web_contents)
    return NULL;

  return TabSpecificContentSettings::FromWebContents(web_contents);
}

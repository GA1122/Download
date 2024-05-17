AwBrowserPermissionRequestDelegate* AwBrowserPermissionRequestDelegate::FromID(
    int render_process_id, int render_frame_id) {
  AwContents* aw_contents = AwContents::FromWebContents(
      content::WebContents::FromRenderFrameHost(
          content::RenderFrameHost::FromID(render_process_id,
                                           render_frame_id)));
  return aw_contents;
}

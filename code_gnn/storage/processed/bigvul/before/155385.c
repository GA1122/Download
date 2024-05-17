WebContents* GetWebContentsFromProcessAndFrameId(int render_process_id,
                                                 int render_frame_id) {
  if (render_process_id) {
    RenderFrameHost* rfh =
        RenderFrameHost::FromID(render_process_id, render_frame_id);
    return WebContents::FromRenderFrameHost(rfh);
  }
  return WebContents::FromFrameTreeNodeId(render_frame_id);
}

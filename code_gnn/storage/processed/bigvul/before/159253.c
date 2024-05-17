WebContents* GetWebContents(int render_process_id,
                            int render_frame_id,
                            int frame_tree_node_id) {
  DCHECK(IsBrowserSideNavigationEnabled());

  WebContents* web_contents = WebContents::FromRenderFrameHost(
      RenderFrameHost::FromID(render_process_id, render_frame_id));
  if (web_contents)
    return web_contents;

  return WebContents::FromFrameTreeNodeId(frame_tree_node_id);
}

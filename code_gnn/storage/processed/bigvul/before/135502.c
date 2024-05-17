content::ResourceRequestInfo::WebContentsGetter GetWebContentsGetter(
    content::WebContents* web_contents) {
  int frame_tree_node_id = web_contents->GetMainFrame()->GetFrameTreeNodeId();
  if (frame_tree_node_id != -1) {
    return base::Bind(content::WebContents::FromFrameTreeNodeId,
                      frame_tree_node_id);
  }

  return base::Bind(&GetWebContentsByFrameID,
                    web_contents->GetMainFrame()->GetProcess()->GetID(),
                    web_contents->GetMainFrame()->GetRoutingID());
}

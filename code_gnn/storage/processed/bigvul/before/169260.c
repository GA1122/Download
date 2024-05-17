bool IsInnerInterstitialPageConnected(InterstitialPage* interstitial_page) {
  InterstitialPageImpl* impl =
      static_cast<InterstitialPageImpl*>(interstitial_page);

  RenderWidgetHostViewBase* rwhvb =
      static_cast<RenderWidgetHostViewBase*>(impl->GetView());
  EXPECT_TRUE(rwhvb->IsRenderWidgetHostViewChildFrame());
  RenderWidgetHostViewChildFrame* rwhvcf =
      static_cast<RenderWidgetHostViewChildFrame*>(rwhvb);

  CrossProcessFrameConnector* frame_connector =
      static_cast<CrossProcessFrameConnector*>(
          rwhvcf->FrameConnectorForTesting());

  WebContentsImpl* inner_web_contents =
      static_cast<WebContentsImpl*>(impl->GetWebContents());
  FrameTreeNode* outer_node = FrameTreeNode::GloballyFindByID(
      inner_web_contents->GetOuterDelegateFrameTreeNodeId());

  return outer_node->current_frame_host()->GetView() ==
         frame_connector->GetParentRenderWidgetHostView();
}

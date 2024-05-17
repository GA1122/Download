void WaitForAccessibilityTreeToContainNodeWithName(WebContents* web_contents,
                                                   const std::string& name) {
  WebContentsImpl* web_contents_impl = static_cast<WebContentsImpl*>(
      web_contents);
  RenderFrameHostImpl* main_frame = static_cast<RenderFrameHostImpl*>(
      web_contents_impl->GetMainFrame());
  BrowserAccessibilityManager* main_frame_manager =
      main_frame->browser_accessibility_manager();
  FrameTree* frame_tree = web_contents_impl->GetFrameTree();
  while (!main_frame_manager || !AccessibilityTreeContainsNodeWithName(
             main_frame_manager->GetRoot(), name)) {
    AccessibilityNotificationWaiter accessibility_waiter(
        main_frame, ax::mojom::Event::kNone);
    for (FrameTreeNode* node : frame_tree->Nodes()) {
      accessibility_waiter.ListenToAdditionalFrame(
          node->current_frame_host());
    }

    content::BrowserPluginGuestManager* guest_manager =
        web_contents_impl->GetBrowserContext()->GetGuestManager();
    if (guest_manager) {
      guest_manager->ForEachGuest(web_contents_impl,
                                  base::BindRepeating(&ListenToGuestWebContents,
                                                      &accessibility_waiter));
    }

    accessibility_waiter.WaitForNotification();
    main_frame_manager = main_frame->browser_accessibility_manager();
  }
}

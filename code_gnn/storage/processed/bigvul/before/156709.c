  void SetupTest() {
    GURL test_url(embedded_test_server()->GetURL(
        "a.com", "/cross_site_iframe_factory.html?a(a)"));
    EXPECT_TRUE(NavigateToURL(shell(), test_url));
    frame_observer_ = std::make_unique<RenderFrameSubmissionObserver>(
        shell()->web_contents());
    FrameTreeNode* root = static_cast<WebContentsImpl*>(shell()->web_contents())
                              ->GetFrameTree()
                              ->root();
    EXPECT_EQ(
        " Site A\n"
        "   +--Site A\n"
        "Where A = http://a.com/",
        FrameTreeVisualizer().DepictFrameTree(root));
    TestNavigationObserver observer(shell()->web_contents());
    EXPECT_EQ(1u, root->child_count());
    child_frame_tree_node_ = root->child_at(0);

    root_rwhv_ = static_cast<RenderWidgetHostViewAndroid*>(
        root->current_frame_host()->GetRenderWidgetHost()->GetView());
    selection_controller_client_ =
        new TouchSelectionControllerClientTestWrapper(
            root_rwhv_->GetSelectionControllerClientManagerForTesting());
    root_rwhv_->SetSelectionControllerClientForTesting(
        base::WrapUnique(selection_controller_client_));

    GURL child_url(
        embedded_test_server()->GetURL("b.com", "/touch_selection.html"));
    NavigateFrameToURL(child_frame_tree_node_, child_url);
    EXPECT_EQ(
        " Site A ------------ proxies for B\n"
        "   +--Site B ------- proxies for A\n"
        "Where A = http://a.com/\n"
        "      B = http://b.com/",
        FrameTreeVisualizer().DepictFrameTree(root));
    child_frame_tree_node_ = root->child_at(0);
    WaitForHitTestDataOrChildSurfaceReady(
        child_frame_tree_node_->current_frame_host());

    child_rwhv_ = static_cast<RenderWidgetHostViewChildFrame*>(
        child_frame_tree_node_->current_frame_host()
            ->GetRenderWidgetHost()
            ->GetView());

    EXPECT_EQ(child_url, observer.last_navigation_url());
    EXPECT_TRUE(observer.last_navigation_succeeded());
    FrameStableObserver child_frame_stable_observer(
        child_rwhv_, TestTimeouts::tiny_timeout());
    child_frame_stable_observer.WaitUntilStable();
  }

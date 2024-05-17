  void LoadPage() {
    ASSERT_TRUE(NavigateToURL(
        shell(),
        GURL(embedded_test_server()->GetURL(
            "a.com", "/cross_site_iframe_factory.html?a(b,c(a(b)))"))));
    FrameTreeNode* root = web_contents()->GetFrameTree()->root();
    frames_.push_back(root->current_frame_host());
    frames_.push_back(root->child_at(0)->current_frame_host());
    frames_.push_back(root->child_at(1)->current_frame_host());
    frames_.push_back(root->child_at(1)->child_at(0)->current_frame_host());
    frames_.push_back(
        root->child_at(1)->child_at(0)->child_at(0)->current_frame_host());

    std::string add_input_script =
        "var input = document.createElement('input');"
        "document.body.appendChild(input);"
        "window.oninput = function() {"
        "  input.select();"
        "};";

    for (auto* frame : frames_)
      ASSERT_TRUE(ExecuteScript(frame, add_input_script));
  }

void DevToolsWindow::WebContentsCreated(WebContents* source_contents,
                                        int opener_render_process_id,
                                        int opener_render_frame_id,
                                        const std::string& frame_name,
                                        const GURL& target_url,
                                        WebContents* new_contents) {
  if (target_url.SchemeIs(content::kChromeDevToolsScheme) &&
      target_url.path().rfind("toolbox.html") != std::string::npos) {
    CHECK(can_dock_);
    if (toolbox_web_contents_)
      delete toolbox_web_contents_;
    toolbox_web_contents_ = new_contents;

    task_manager::WebContentsTags::CreateForDevToolsContents(
        toolbox_web_contents_);
    data_use_measurement::DataUseWebContentsObserver::CreateForWebContents(
        toolbox_web_contents_);
  }
}

void Browser::WebContentsCreated(WebContents* source_contents,
                                 int opener_render_process_id,
                                 int opener_render_frame_id,
                                 const std::string& frame_name,
                                 const GURL& target_url,
                                 WebContents* new_contents) {
  TabHelpers::AttachTabHelpers(new_contents);

  task_manager::WebContentsTags::CreateForTabContents(new_contents);
}

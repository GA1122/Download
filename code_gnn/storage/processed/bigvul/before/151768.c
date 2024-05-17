void Browser::WebContentsCreated(WebContents* source_contents,
                                 int opener_render_process_id,
                                 int opener_render_frame_id,
                                 const std::string& frame_name,
                                 const GURL& target_url,
                                 WebContents* new_contents) {
  TabHelpers::AttachTabHelpers(new_contents);

  task_manager::WebContentsTags::CreateForTabContents(new_contents);

  RetargetingDetails details;
  details.source_web_contents = source_contents;
  details.source_render_process_id = opener_render_process_id;
  details.source_render_frame_id = opener_render_frame_id;
  details.target_url = target_url;
  details.target_web_contents = new_contents;
  details.not_yet_in_tabstrip = true;
  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_RETARGETING,
      content::Source<Profile>(profile_),
      content::Details<RetargetingDetails>(&details));
}

void Browser::WebContentsCreated(WebContents* source_contents,
                                 int64 source_frame_id,
                                 const GURL& target_url,
                                 WebContents* new_contents) {
  AdoptAsTabContents(new_contents);

  RetargetingDetails details;
  details.source_web_contents = source_contents;
  details.source_frame_id = source_frame_id;
  details.target_url = target_url;
  details.target_web_contents = new_contents;
  details.not_yet_in_tabstrip = true;
  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_RETARGETING,
      content::Source<Profile>(profile_),
      content::Details<RetargetingDetails>(&details));
}

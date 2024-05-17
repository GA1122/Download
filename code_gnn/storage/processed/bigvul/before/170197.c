  content::MediaStreamRequest CreateRequest(
      content::MediaStreamType audio_request_type,
      content::MediaStreamType video_request_type) {
    content::WebContents* web_contents =
        browser()->tab_strip_model()->GetActiveWebContents();
    EXPECT_EQ(request_url_,
              web_contents->GetMainFrame()->GetLastCommittedURL());
    int render_process_id = web_contents->GetMainFrame()->GetProcess()->GetID();
    int render_frame_id = web_contents->GetMainFrame()->GetRoutingID();
    return content::MediaStreamRequest(
        render_process_id, render_frame_id, 0, request_url_.GetOrigin(), false,
        content::MEDIA_DEVICE_ACCESS, std::string(), std::string(),
        audio_request_type, video_request_type, false);
  }

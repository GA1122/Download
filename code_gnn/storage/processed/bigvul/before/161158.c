  void SetCapturingLinkSecured(bool is_secure) {
    MediaObserver* media_observer =
        GetContentClient()->browser()->GetMediaObserver();
    if (!media_observer)
      return;

    media_observer->OnSetCapturingLinkSecured(target_process_id_,
                                              target_frame_id_, page_request_id,
                                              video_type_, is_secure);
  }

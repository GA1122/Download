RenderFrameSubmissionObserver::RenderFrameSubmissionObserver(
    WebContents* web_contents)
    : RenderFrameSubmissionObserver(
          RenderFrameMetadataProviderFromWebContents(web_contents)) {}

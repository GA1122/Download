  FullscreenWebContentsObserver(WebContents* web_contents,
                                RenderFrameHost* wanted_rfh)
      : WebContentsObserver(web_contents), wanted_rfh_(wanted_rfh) {}

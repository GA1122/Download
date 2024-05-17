  BeginNavigationInitiatorReplacer(
      WebContents* web_contents,
      base::Optional<url::Origin> initiator_to_inject)
      : FrameHostInterceptor(web_contents),
        initiator_to_inject_(initiator_to_inject) {}

  OwnerContentsObserver(GuestViewBase* guest,
                        content::WebContents* embedder_web_contents)
      : WebContentsObserver(embedder_web_contents),
        is_fullscreen_(false),
        destroyed_(false),
        guest_(guest) {}

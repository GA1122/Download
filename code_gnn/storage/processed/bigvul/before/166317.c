  ContentsObserver(WebContents* web_contents,
                   FrameDeletionObserver* parent_observer)
      : WebContentsObserver(web_contents), parent_observer_(parent_observer) {}

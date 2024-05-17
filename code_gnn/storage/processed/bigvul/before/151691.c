  InterstitialObserver(Browser* browser, content::WebContents* web_contents)
      : WebContentsObserver(web_contents),
        browser_(browser) {
  }

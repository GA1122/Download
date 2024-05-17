  explicit MockScreenshotManager(content::NavigationControllerImpl* owner)
      : content::WebContentsScreenshotManager(owner),
        encoding_screenshot_in_progress_(false) {
  }

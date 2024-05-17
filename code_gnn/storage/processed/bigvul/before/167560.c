  WindowDestroyer(content::WebContents* web_contents, TabStripModel* model)
      : content::WebContentsObserver(web_contents),
        tab_strip_model_(model),
        browser_closed_observer_(chrome::NOTIFICATION_BROWSER_CLOSED,
                                 content::NotificationService::AllSources()) {}

  HistoryNavigationBeforeCommitInjector(WebContentsImpl* web_contents,
                                        const GURL& url)
      : DidCommitProvisionalLoadInterceptor(web_contents),
        did_trigger_history_navigation_(false),
        url_(url) {}

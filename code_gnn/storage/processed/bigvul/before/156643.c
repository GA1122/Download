  CommitMessageOrderReverser(
      WebContents* web_contents,
      const GURL& deferred_url,
      DidStartDeferringCommitCallback deferred_url_triggered_action)
      : DidCommitProvisionalLoadInterceptor(web_contents),
        deferred_url_(deferred_url),
        deferred_url_triggered_action_(
            std::move(deferred_url_triggered_action)) {}

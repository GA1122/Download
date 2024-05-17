void WebLocalFrameImpl::DidFail(const ResourceError& error,
                                bool was_provisional,
                                HistoryCommitType commit_type) {
  if (!Client())
    return;
  WebURLError web_error = error;
  WebHistoryCommitType web_commit_type =
      static_cast<WebHistoryCommitType>(commit_type);

  if (WebPluginContainerImpl* plugin = GetFrame()->GetWebPluginContainer())
    plugin->DidFailLoading(error);

  if (was_provisional)
    Client()->DidFailProvisionalLoad(web_error, web_commit_type);
  else
    Client()->DidFailLoad(web_error, web_commit_type);
}

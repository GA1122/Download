void WebContentsImpl::OnWebContentsDestroyed(WebContentsImpl* web_contents) {
  RemoveDestructionObserver(web_contents);

  for (auto iter = pending_contents_.begin(); iter != pending_contents_.end();
       ++iter) {
    if (iter->second.get() != web_contents)
      continue;

    iter->second.release();
    pending_contents_.erase(iter);
    return;
  }
  NOTREACHED();
}

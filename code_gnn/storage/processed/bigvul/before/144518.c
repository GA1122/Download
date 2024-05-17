WebContentsImpl* WebContentsImpl::GetCreatedWindow(int route_id) {
  PendingContents::iterator iter = pending_contents_.find(route_id);

  if (iter == pending_contents_.end()) {
    return NULL;
  }

  WebContentsImpl* new_contents = iter->second;
  pending_contents_.erase(route_id);
  RemoveDestructionObserver(new_contents);

  if (BrowserPluginGuest::IsGuest(new_contents))
    return new_contents;

  if (!new_contents->GetRenderProcessHost()->HasConnection() ||
      !new_contents->GetRenderViewHost()->GetWidget()->GetView())
    return NULL;

  return new_contents;
}

std::unique_ptr<WebContents> WebContentsImpl::GetCreatedWindow(
    int process_id,
    int main_frame_widget_route_id) {
  auto key = std::make_pair(process_id, main_frame_widget_route_id);
  auto iter = pending_contents_.find(key);

  if (iter == pending_contents_.end())
    return nullptr;

  std::unique_ptr<WebContents> new_contents = std::move(iter->second);
  pending_contents_.erase(key);
  WebContentsImpl* raw_new_contents =
      static_cast<WebContentsImpl*>(new_contents.get());
  RemoveDestructionObserver(raw_new_contents);

  if (BrowserPluginGuest::IsGuest(raw_new_contents))
    return new_contents;

  if (!new_contents->GetMainFrame()->GetProcess()->IsInitializedAndNotDead() ||
      !new_contents->GetMainFrame()->GetView()) {
    return nullptr;
  }

  return new_contents;
}

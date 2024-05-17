TabContents* RenderViewHostDelegateViewHelper::GetCreatedWindow(int route_id) {
  PendingContents::iterator iter = pending_contents_.find(route_id);
  if (iter == pending_contents_.end()) {
    DCHECK(false);
    return NULL;
  }

  TabContents* new_tab_contents = iter->second;
  pending_contents_.erase(route_id);

  if (!new_tab_contents->render_view_host()->view() ||
      !new_tab_contents->process()->HasConnection()) {
    return NULL;
  }

  new_tab_contents->render_view_host()->Init();

  return new_tab_contents;
}

void WebContentsImpl::DidStopLoading(RenderViewHost* render_view_host) {
  scoped_ptr<LoadNotificationDetails> details;

  NavigationEntry* entry = controller_.GetLastCommittedEntry();

  if (entry) {
    base::TimeDelta elapsed = base::TimeTicks::Now() - current_load_start_;

    details.reset(new LoadNotificationDetails(
        entry->GetVirtualURL(),
        entry->GetTransitionType(),
        elapsed,
        &controller_,
        controller_.GetCurrentEntryIndex()));
  }

  SetIsLoading(render_view_host, false, details.get());
}

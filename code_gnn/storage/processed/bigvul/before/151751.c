void Browser::TabDetachedAtImpl(content::WebContents* contents,
                                int index,
                                DetachType type) {
  if (type == DETACH_TYPE_DETACH) {
    if (contents == tab_strip_model_->GetActiveWebContents()) {
      LocationBar* location_bar = window()->GetLocationBar();
      if (location_bar)
        location_bar->SaveStateToContents(contents);
    }

    if (!tab_strip_model_->closing_all())
      SyncHistoryWithTabs(0);
  }

  SetAsDelegate(contents, false);
  RemoveScheduledUpdatesFor(contents);

  if (find_bar_controller_.get() && index == tab_strip_model_->active_index()) {
    find_bar_controller_->ChangeWebContents(NULL);
  }

  search_delegate_->OnTabDetached(contents);

  for (size_t i = 0; i < interstitial_observers_.size(); i++) {
    if (interstitial_observers_[i]->web_contents() != contents)
      continue;

    delete interstitial_observers_[i];
    interstitial_observers_.erase(interstitial_observers_.begin() + i);
    return;
  }
}

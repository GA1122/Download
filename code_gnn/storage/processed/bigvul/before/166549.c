void BrowserCommandController::OnTabStripModelChanged(
    TabStripModel* tab_strip_model,
    const TabStripModelChange& change,
    const TabStripSelectionChange& selection) {
  if (change.type() != TabStripModelChange::kInserted &&
      change.type() != TabStripModelChange::kReplaced &&
      change.type() != TabStripModelChange::kRemoved)
    return;

  for (const auto& delta : change.deltas()) {
    content::WebContents* new_contents = nullptr;
    content::WebContents* old_contents = nullptr;
    if (change.type() == TabStripModelChange::kInserted) {
      new_contents = delta.insert.contents;
    } else if (change.type() == TabStripModelChange::kReplaced) {
      new_contents = delta.replace.new_contents;
      old_contents = delta.replace.old_contents;
    } else {
      old_contents = delta.remove.contents;
    }

    if (old_contents)
      RemoveInterstitialObservers(old_contents);
    if (new_contents)
      AddInterstitialObservers(new_contents);
  }
}

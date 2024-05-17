ManifestChangeNotifier::GetManifestChangeObserver() {
  if (!manifest_change_observer_) {
    render_frame()->GetRemoteAssociatedInterfaces()->GetInterface(
        &manifest_change_observer_);
  }
  return *manifest_change_observer_;
}

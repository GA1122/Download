void ManifestChangeNotifier::ReportManifestChange() {
  auto manifest_url =
      render_frame()->GetWebFrame()->GetDocument().ManifestURL();
  if (manifest_url.IsNull()) {
    GetManifestChangeObserver().ManifestUrlChanged(base::nullopt);
  } else {
    GetManifestChangeObserver().ManifestUrlChanged(GURL(manifest_url));
  }
}

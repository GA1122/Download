void ManifestManager::DidChangeManifest() {
  may_have_manifest_ = true;
  manifest_dirty_ = true;
  manifest_url_ = GURL();
  manifest_debug_info_ = nullptr;
}

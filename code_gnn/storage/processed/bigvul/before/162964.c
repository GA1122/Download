void ManifestManager::DidCommitProvisionalLoad(
    bool is_new_navigation,
    bool is_same_document_navigation) {
  if (is_same_document_navigation)
    return;

  may_have_manifest_ = false;
  manifest_dirty_ = true;
  manifest_url_ = GURL();
 }

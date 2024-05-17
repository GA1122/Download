void AppCacheHost::OnGroupLoaded(AppCacheGroup* group,
                                 const GURL& manifest_url) {
  DCHECK(manifest_url == pending_selected_manifest_url_);
  pending_selected_manifest_url_ = GURL();
  FinishCacheSelection(NULL, group);
}

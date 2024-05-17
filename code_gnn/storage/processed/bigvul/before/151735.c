bool Browser::ShouldCreateWebContents(
    content::WebContents* web_contents,
    content::SiteInstance* source_site_instance,
    int32_t route_id,
    int32_t main_frame_route_id,
    int32_t main_frame_widget_route_id,
    WindowContainerType window_container_type,
    const GURL& opener_url,
    const std::string& frame_name,
    const GURL& target_url,
    const std::string& partition_id,
    content::SessionStorageNamespace* session_storage_namespace) {
  if (window_container_type == WINDOW_CONTAINER_TYPE_BACKGROUND) {
    return !MaybeCreateBackgroundContents(
        source_site_instance, opener_url, route_id, main_frame_route_id,
        main_frame_widget_route_id, frame_name, target_url, partition_id,
        session_storage_namespace);
  }

  return true;
}

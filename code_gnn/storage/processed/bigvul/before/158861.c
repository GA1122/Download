bool Browser::ShouldCreateWebContents(
    content::WebContents* web_contents,
    content::RenderFrameHost* opener,
    content::SiteInstance* source_site_instance,
    int32_t route_id,
    int32_t main_frame_route_id,
    int32_t main_frame_widget_route_id,
    content::mojom::WindowContainerType window_container_type,
    const GURL& opener_url,
    const std::string& frame_name,
    const GURL& target_url,
    const std::string& partition_id,
    content::SessionStorageNamespace* session_storage_namespace) {
  if (window_container_type ==
      content::mojom::WindowContainerType::BACKGROUND) {
    return !MaybeCreateBackgroundContents(
        source_site_instance, opener, opener_url, route_id, main_frame_route_id,
        main_frame_widget_route_id, frame_name, target_url, partition_id,
        session_storage_namespace);
  }

  return true;
}

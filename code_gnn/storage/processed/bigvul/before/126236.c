bool Browser::ShouldCreateWebContents(
    WebContents* web_contents,
    int route_id,
    WindowContainerType window_container_type,
    const string16& frame_name,
    const GURL& target_url) {
  if (window_container_type == WINDOW_CONTAINER_TYPE_BACKGROUND) {
    return !MaybeCreateBackgroundContents(
        route_id, web_contents, frame_name, target_url);
  }

  return true;
}

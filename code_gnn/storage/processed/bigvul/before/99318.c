void ResourceMessageFilter::OnMsgCreateWindow(
    int opener_id, bool user_gesture,
    WindowContainerType window_container_type,
    int64 session_storage_namespace_id, int* route_id,
    int64* cloned_session_storage_namespace_id) {
  *cloned_session_storage_namespace_id = dom_storage_dispatcher_host_->
      CloneSessionStorage(session_storage_namespace_id);
  render_widget_helper_->CreateNewWindow(opener_id,
                                         user_gesture,
                                         window_container_type,
                                         handle(),
                                         route_id);
}

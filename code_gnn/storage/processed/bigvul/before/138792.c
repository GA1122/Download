void RenderFrameHostImpl::RunCreateWindowCompleteCallback(
    const CreateNewWindowCallback& callback,
    mojom::CreateNewWindowReplyPtr reply,
    int render_view_route_id,
    int main_frame_route_id,
    int main_frame_widget_route_id,
    int cloned_session_storage_namespace_id) {
  reply->route_id = render_view_route_id;
  reply->main_frame_route_id = main_frame_route_id;
  reply->main_frame_widget_route_id = main_frame_widget_route_id;
  reply->cloned_session_storage_namespace_id =
      cloned_session_storage_namespace_id;
  callback.Run(std::move(reply));
}

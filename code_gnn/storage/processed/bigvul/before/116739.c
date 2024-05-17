void MockRenderThread::OnMsgCreateWidget(int opener_id,
                                         WebKit::WebPopupType popup_type,
                                         int* route_id,
                                         int* surface_id) {
  opener_id_ = opener_id;
  *route_id = routing_id_;
   *surface_id = surface_id_;
 }

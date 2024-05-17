void ResourceMessageFilter::OnMsgCreateWidget(int opener_id,
                                              WebKit::WebPopupType popup_type,
                                              int* route_id) {
  render_widget_helper_->CreateNewWidget(opener_id, popup_type, route_id);
}

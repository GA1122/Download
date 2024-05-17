void RenderViewHostImpl::CreateNewWidget(int route_id,
                                     WebKit::WebPopupType popup_type) {
  delegate_->CreateNewWidget(route_id, popup_type);
}

WebExternalPopupMenu* RenderFrameImpl::CreateExternalPopupMenu(
    const WebPopupMenuInfo& popup_menu_info,
    WebExternalPopupMenuClient* popup_menu_client) {
#if BUILDFLAG(USE_EXTERNAL_POPUP_MENU)
  if (external_popup_menu_)
    return NULL;
  external_popup_menu_.reset(
      new ExternalPopupMenu(this, popup_menu_info, popup_menu_client));
  render_view_->GetWidget()->SetExternalPopupOriginAdjustmentsForEmulation(
      external_popup_menu_.get());
  return external_popup_menu_.get();
#else
  return nullptr;
#endif
}

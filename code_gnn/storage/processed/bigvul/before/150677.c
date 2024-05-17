void PageInfo::OnSiteChosenObjectDeleted(const ChooserUIInfo& ui_info,
                                         const base::Value& object) {
  ChooserContextBase* context = ui_info.get_context(profile_);
  const auto origin = url::Origin::Create(site_url_);
  context->RevokeObjectPermission(origin, origin, object);
  show_info_bar_ = true;

  PresentSitePermissions();
}

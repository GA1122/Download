void PageInfo::PresentPageFeatureInfo() {
  PageInfoUI::PageFeatureInfo info;
  info.is_vr_presentation_in_headset =
      vr::VrTabHelper::IsContentDisplayedInHeadset(web_contents());

  ui_->SetPageFeatureInfo(info);
}

void UiSceneCreator::CreateUnderDevelopmentNotice() {
  auto text = base::MakeUnique<Text>(kUnderDevelopmentNoticeFontHeightDMM);
  BindColor(model_, text.get(), &ColorScheme::world_background_text,
            &Text::SetColor);
  text->SetText(l10n_util::GetStringUTF16(IDS_VR_UNDER_DEVELOPMENT_NOTICE));
  text->SetName(kUnderDevelopmentNotice);
  text->SetDrawPhase(kPhaseForeground);
  text->set_hit_testable(false);
  text->SetSize(kUnderDevelopmentNoticeWidthDMM,
                kUnderDevelopmentNoticeHeightDMM);
  text->SetTranslate(0, -kUnderDevelopmentNoticeVerticalOffsetDMM, 0);
  text->SetRotate(1, 0, 0, kUnderDevelopmentNoticeRotationRad);
  text->set_y_anchoring(BOTTOM);
  scene_->AddUiElement(kUrlBar, std::move(text));
}

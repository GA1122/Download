void TranslateManager::ShowInfoBar(TabContents* tab,
                                   TranslateInfoBarDelegate* infobar) {
  TranslateInfoBarDelegate* old_infobar = GetTranslateInfoBarDelegate(tab);
  infobar->UpdateBackgroundAnimation(old_infobar);
  if (old_infobar) {
    tab->ReplaceInfoBar(old_infobar, infobar);
  } else {
    tab->AddInfoBar(infobar);
  }
}

  TranslateInfoBarDelegate* TranslateManager::GetTranslateInfoBarDelegate(
      TabContents* tab) {
    for (int i = 0; i < tab->infobar_delegate_count(); ++i) {
    InfoBarDelegate* delegate = tab->GetInfoBarDelegateAt(i);
    if (!delegate) {
      NOTREACHED();
      continue;
    }
    TranslateInfoBarDelegate* translate_delegate =
        delegate->AsTranslateInfoBarDelegate();
    if (translate_delegate)
      return translate_delegate;
//     TranslateInfoBarDelegate* delegate =
//         tab->GetInfoBarDelegateAt(i)->AsTranslateInfoBarDelegate();
//     if (delegate)
//       return delegate;
    }
    return NULL;
  }
void InfoBarContainer::ReplaceInfoBar(InfoBarDelegate* old_delegate,
                                      InfoBarDelegate* new_delegate) {
  RemoveInfoBar(old_delegate, false);   
  AddInfoBar(new_delegate, false);   
}

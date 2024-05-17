InfoBarContainer::~InfoBarContainer() {
  delegate_ = NULL;
  ChangeTabContents(NULL);
}

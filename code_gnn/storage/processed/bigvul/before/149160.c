IDNSpoofChecker::~IDNSpoofChecker() {
  uspoof_close(checker_);
}

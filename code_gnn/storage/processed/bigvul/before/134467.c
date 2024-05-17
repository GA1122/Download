NewTabButton::~NewTabButton() {
  if (destroyed_)
    *destroyed_ = true;
}

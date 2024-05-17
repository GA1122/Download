ScreenLayoutObserver::~ScreenLayoutObserver() {
  ShellPort::Get()->RemoveDisplayObserver(this);
}

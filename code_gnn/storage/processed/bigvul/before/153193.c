void DesktopWindowTreeHostX11::AddObserver(
    DesktopWindowTreeHostObserverX11* observer) {
  observer_list_.AddObserver(observer);
}

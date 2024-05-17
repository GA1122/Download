void DesktopWindowTreeHostX11::RemoveObserver(
    DesktopWindowTreeHostObserverX11* observer) {
  observer_list_.RemoveObserver(observer);
}

void FakeCrosDisksClient::RemoveObserver(Observer* observer) {
  observer_list_.RemoveObserver(observer);
}

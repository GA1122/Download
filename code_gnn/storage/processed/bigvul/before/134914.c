void ChromeBrowserMainPartsChromeos::PreMainMessageLoopStart() {
  net::NetworkChangeNotifier::SetFactory(
      new NetworkChangeNotifierFactoryChromeos());
  ChromeBrowserMainPartsLinux::PreMainMessageLoopStart();
}

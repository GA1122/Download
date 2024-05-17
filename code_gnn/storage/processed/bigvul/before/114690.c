  void ShowInterstitial(const char* url) {
    new TestOfflineLoadPage(contents(), GURL(url), this);
  }

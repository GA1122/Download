  explicit AndroidIncognitoObserver(ChromeMetricsServiceClient* parent)
      : parent_(parent) {
    TabModelList::AddObserver(this);
  }

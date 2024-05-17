  base::Callback<void(bool)> const can_download_callback() {
    return base::Bind(&BackgroundLoaderOfflinerTest::OnCanDownload,
                      base::Unretained(this));
  }

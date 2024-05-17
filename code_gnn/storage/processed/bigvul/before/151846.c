  Offliner::ProgressCallback const progress_callback() {
    return base::BindRepeating(&BackgroundLoaderOfflinerTest::OnProgress,
                               base::Unretained(this));
  }

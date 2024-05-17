  Offliner::CompletionCallback completion_callback() {
    return base::BindOnce(&BackgroundLoaderOfflinerTest::OnCompletion,
                          base::Unretained(this));
  }

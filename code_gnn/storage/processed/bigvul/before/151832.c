  Offliner::CancelCallback cancel_callback() {
    return base::BindOnce(&BackgroundLoaderOfflinerTest::OnCancel,
                          base::Unretained(this));
  }

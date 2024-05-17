  void FailLoading() {
    data_provider()->DidFail(response_generator_->GenerateError());
    base::RunLoop().RunUntilIdle();
  }

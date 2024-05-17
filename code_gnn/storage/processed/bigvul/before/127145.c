  void SetCards(std::vector<CreditCard>* cards) {
    WindowedPersonalDataManagerObserver observer(browser());
    personal_data_manager()->SetCreditCards(cards);
    observer.Wait();
  }

  void SetProfiles(std::vector<AutofillProfile>* profiles) {
    WindowedPersonalDataManagerObserver observer(browser());
    GetPersonalDataManager()->SetProfiles(profiles);
    observer.Wait();
  }

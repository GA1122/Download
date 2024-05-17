  PersonalDataManager* GetPersonalDataManager() {
    return PersonalDataManagerFactory::GetForProfile(browser()->profile());
  }

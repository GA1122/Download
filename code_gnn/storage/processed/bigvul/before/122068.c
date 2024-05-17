void ProfileDependencyManager::CreateProfileServices(Profile* profile,
                                                     bool is_testing_profile) {
#ifndef NDEBUG
  dead_profile_pointers_.erase(profile);
#endif

  AssertFactoriesBuilt();

  if (destruction_order_.empty())
    BuildDestructionOrder(profile);

  for (std::vector<ProfileKeyedBaseFactory*>::reverse_iterator rit =
           destruction_order_.rbegin(); rit != destruction_order_.rend();
       ++rit) {
    if (!profile->IsOffTheRecord()) {
      (*rit)->RegisterUserPrefsOnProfile(profile);
    }

    if (is_testing_profile && (*rit)->ServiceIsNULLWhileTesting()) {
      (*rit)->SetEmptyTestingFactory(profile);
    } else if ((*rit)->ServiceIsCreatedWithProfile()) {
      (*rit)->CreateServiceNow(profile);
    }
  }
}

  static void LinkProfiles(OtrTestingProfile* profile1,
                           OtrTestingProfile* profile2) {
    profile1->set_linked_profile(profile2);
    profile2->set_linked_profile(profile1);
  }

  void DisableIds(const ObjectIdSet& oids) {
    for (ObjectIdSet::const_iterator it = oids.begin(); it != oids.end();
         ++it) {
      fake_invalidation_client_.LoseRegistration(*it);
      fake_registration_manager_.DisableId(*it);
    }
  }

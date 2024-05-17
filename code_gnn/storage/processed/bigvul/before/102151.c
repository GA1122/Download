  bool InitialSyncEndedForAllEnabledTypes() {
    syncable::ModelTypeSet types;
    ModelSafeRoutingInfo enabled_types;
    registrar_->GetModelSafeRoutingInfo(&enabled_types);
    for (ModelSafeRoutingInfo::const_iterator i = enabled_types.begin();
        i != enabled_types.end(); ++i) {
      types.insert(i->first);
    }

    return InitialSyncEndedForTypes(types, &share_);
  }

  static NudgeDelayStrategy GetNudgeDelayStrategy(const ModelType& type) {
    switch (type) {
     case syncable::AUTOFILL:
       return ACCOMPANY_ONLY;
     case syncable::PREFERENCES:
     case syncable::SESSIONS:
       return CUSTOM;
     default:
       return IMMEDIATE;
    }
  }

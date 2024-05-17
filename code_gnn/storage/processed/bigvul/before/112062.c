  Cryptographer* cryptographer(syncable::BaseTransaction* trans) {
    return directory()->GetCryptographer(trans);
  }

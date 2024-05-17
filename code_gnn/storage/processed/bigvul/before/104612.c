int GetLocationRank(Extension::Location location) {
  const int kInvalidRank = -1;
  int rank = kInvalidRank;   

  switch (location) {
    case Extension::COMPONENT:
      rank = 6;
      break;

    case Extension::EXTERNAL_POLICY_DOWNLOAD:
      rank = 5;
      break;

    case Extension::LOAD:
      rank = 4;
      break;

    case Extension::EXTERNAL_REGISTRY:
      rank = 3;
      break;

    case Extension::EXTERNAL_PREF:
      rank = 2;
      break;

    case Extension::EXTERNAL_PREF_DOWNLOAD:
      rank = 1;
      break;

    case Extension::INTERNAL:
      rank = 0;
      break;

    default:
      NOTREACHED() << "Need to add new extension locaton " << location;
  }

  CHECK(rank != kInvalidRank);
  return rank;
}

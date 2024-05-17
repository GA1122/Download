Extension::Location Extension::GetHigherPriorityLocation(
    Extension::Location loc1, Extension::Location loc2) {
  if (loc1 == loc2)
    return loc1;

  int loc1_rank = GetLocationRank(loc1);
  int loc2_rank = GetLocationRank(loc2);

  CHECK(loc1_rank != loc2_rank);

  return (loc1_rank > loc2_rank ? loc1 : loc2 );
}

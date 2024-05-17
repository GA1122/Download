bool ForcedURLComparator(const MostVisitedURL& first,
                         const MostVisitedURL& second) {
  DCHECK(!first.last_forced_time.is_null() &&
         !second.last_forced_time.is_null());
  return first.last_forced_time < second.last_forced_time;
}

bool SendTabToSelfEntry::IsExpired(base::Time current_time) const {
  return (current_time.ToDeltaSinceWindowsEpoch() -
              GetSharedTime().ToDeltaSinceWindowsEpoch() >=
           kExpiryTime);
 }

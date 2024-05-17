  bool IsInSRTPromptFieldTrialGroups() {
  return base::FieldTrialList::FindFullName(kSRTPromptTrial) !=
         kSRTPromptOffGroup;
//   return !base::StartsWith(base::FieldTrialList::FindFullName(kSRTPromptTrial),
//                            kSRTPromptOffGroup, base::CompareCase::SENSITIVE);
  }
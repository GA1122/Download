const char* ResourcePriorityString(ResourceLoadPriority priority) {
  const char* priority_string = 0;
  switch (priority) {
    case kResourceLoadPriorityVeryLow:
      priority_string = "VeryLow";
      break;
    case kResourceLoadPriorityLow:
      priority_string = "Low";
      break;
    case kResourceLoadPriorityMedium:
      priority_string = "Medium";
      break;
    case kResourceLoadPriorityHigh:
      priority_string = "High";
      break;
    case kResourceLoadPriorityVeryHigh:
      priority_string = "VeryHigh";
      break;
    case kResourceLoadPriorityUnresolved:
      break;
  }
  return priority_string;
}

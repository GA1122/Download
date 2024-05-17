bool IsUnsupportedProcessModel(ProcessModel process_model) {
  switch (process_model) {
    case PROCESS_MODEL_MULTI_PROCESS:
    case PROCESS_MODEL_SINGLE_PROCESS:
    case PROCESS_MODEL_PROCESS_PER_SITE_INSTANCE:
      return false;
    default:
      return true;
  }
}

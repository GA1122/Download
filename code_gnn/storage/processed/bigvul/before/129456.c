  void LogClientServiceForInfo(
      T* info, GLuint client_id, const char* function_name) {
    if (info) {
      LogClientServiceMapping(function_name, client_id, info->service_id());
    }
  }

bool ReceiveCreateProcessResponse(
    HANDLE pipe,
    PROCESS_INFORMATION* process_information_out) {
  struct CreateProcessResponse {
    DWORD size;
    BOOL success;
    DWORD last_error;
    PROCESS_INFORMATION process_information;
  };

  DWORD bytes;
  CreateProcessResponse response;
  if (!ReadFile(pipe, &response, sizeof(response), &bytes, NULL)) {
    LOG_GETLASTERROR(ERROR) << "Failed to receive CreateProcessAsUser response";
    return false;
  }

  if (bytes != sizeof(response)) {
    SetLastError(ERROR_RECEIVE_PARTIAL);
    return false;
  }

  if (!response.success) {
    SetLastError(response.last_error);
    return false;
  }

  *process_information_out = response.process_information;
  return true;
}

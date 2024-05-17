void CopyOutputRequestCallbackRunsOnceCallback(
    int* n_called,
    std::unique_ptr<CopyOutputResult> result) {
  ++*n_called;
}

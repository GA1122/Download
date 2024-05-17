void SuccessReportingCallback(
    int* expected_calls,
    const base::RepeatingCallback<void(blink::ServiceWorkerStatusCode)>&
        callback,
    blink::ServiceWorkerStatusCode status) {
  if (status != blink::ServiceWorkerStatusCode::kOk) {
    *expected_calls = -1;
    callback.Run(blink::ServiceWorkerStatusCode::kErrorFailed);
    return;
  }
  (*expected_calls)--;
  if (*expected_calls == 0) {
    callback.Run(blink::ServiceWorkerStatusCode::kOk);
  }
}

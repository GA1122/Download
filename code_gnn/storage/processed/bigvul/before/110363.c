void CallOnMainThread(int delay_in_msec,
                      PP_CompletionCallback callback,
                      int32_t result) {
  if (callback.func) {
    GetMainThreadMessageLoop()->PostDelayedTask(
        FROM_HERE,
        base::Bind(callback.func, callback.user_data, result),
        base::TimeDelta::FromMilliseconds(delay_in_msec));
  }
}

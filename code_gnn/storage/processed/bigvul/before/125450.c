  static void Run(scoped_refptr<base::MessageLoopProxy> relay_proxy,
                  const base::Callback<void(T1)>& callback,
                  T1 arg1) {
    if (callback.is_null())
      return;
    RunTaskOnThread(relay_proxy, base::Bind(callback, arg1));
  }

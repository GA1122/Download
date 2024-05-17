  static void Run(scoped_refptr<base::MessageLoopProxy> relay_proxy,
                  const base::Callback<void(T1, scoped_ptr<T2>)>& callback,
                  T1 arg1,
                  scoped_ptr<T2> arg2) {
    if (callback.is_null())
      return;
    RunTaskOnThread(relay_proxy,
                    base::Bind(callback, arg1, base::Passed(&arg2)));
  }

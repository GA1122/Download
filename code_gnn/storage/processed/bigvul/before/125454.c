  static void Run(scoped_refptr<base::MessageLoopProxy> relay_proxy,
                  const base::Callback<void(T1, T2, scoped_ptr<T3>)>& callback,
                  T1 arg1,
                  T2 arg2,
                  scoped_ptr<T3> arg3) {
    if (callback.is_null())
      return;
    RunTaskOnThread(relay_proxy,
                    base::Bind(callback, arg1, arg2, base::Passed(&arg3)));
  }

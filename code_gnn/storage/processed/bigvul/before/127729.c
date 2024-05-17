void MockNetworkTransaction::CallbackLater(const CompletionCallback& callback,
                                           int result) {
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::Bind(&MockNetworkTransaction::RunCallback,
                            weak_factory_.GetWeakPtr(), callback, result));
}

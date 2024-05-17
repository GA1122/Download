void ProvideWorkerFetchContextToWorker(
    WorkerClients* clients,
    std::unique_ptr<WebWorkerFetchContext> web_context) {
  DCHECK(clients);
  WorkerFetchContextHolder::ProvideTo(
      *clients, WorkerFetchContextHolder::SupplementName(),
      new WorkerFetchContextHolder(std::move(web_context)));
}

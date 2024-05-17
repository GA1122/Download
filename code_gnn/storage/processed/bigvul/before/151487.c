WorkerFetchContext* WorkerFetchContext::Create(
    WorkerOrWorkletGlobalScope& global_scope) {
  DCHECK(global_scope.IsContextThread());
  DCHECK(!global_scope.IsMainThreadWorkletGlobalScope());
  WorkerClients* worker_clients = global_scope.Clients();
  DCHECK(worker_clients);
  WorkerFetchContextHolder* holder =
      static_cast<WorkerFetchContextHolder*>(Supplement<WorkerClients>::From(
          *worker_clients, WorkerFetchContextHolder::SupplementName()));
  if (!holder)
    return nullptr;
  std::unique_ptr<WebWorkerFetchContext> web_context = holder->TakeContext();
  DCHECK(web_context);
  return new WorkerFetchContext(global_scope, std::move(web_context));
}

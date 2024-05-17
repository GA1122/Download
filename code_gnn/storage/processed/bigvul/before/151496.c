  static WorkerFetchContextHolder* From(WorkerClients& clients) {
    return static_cast<WorkerFetchContextHolder*>(
        Supplement<WorkerClients>::From(clients, SupplementName()));
  }

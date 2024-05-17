    Params(
        const scoped_refptr<base::SingleThreadTaskRunner>& worker_task_runner,
        int* num_outstanding_callbacks)
        : v8_resolver(nullptr),
          worker_task_runner(worker_task_runner),
          num_outstanding_callbacks(num_outstanding_callbacks) {}

  void PushNextTask(base::OnceClosure task) {
    task_stack_.push(std::move(task));
  }

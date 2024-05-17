  MouseMoveTask(Task* next_task, int absolute_x, int absolute_y)
      : next_task_(next_task),
        x_(absolute_x),
        y_(absolute_y) {
  }

void wake_up_q(struct wake_q_head *head)
{
	struct wake_q_node *node = head->first;

	while (node != WAKE_Q_TAIL) {
		struct task_struct *task;

		task = container_of(node, struct task_struct, wake_q);
		BUG_ON(!task);
		 
		node = node->next;
		task->wake_q.next = NULL;

		 
		wake_up_process(task);
		put_task_struct(task);
	}
}

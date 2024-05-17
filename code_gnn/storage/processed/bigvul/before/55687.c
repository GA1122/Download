void wake_q_add(struct wake_q_head *head, struct task_struct *task)
{
	struct wake_q_node *node = &task->wake_q;

	 
	if (cmpxchg(&node->next, NULL, WAKE_Q_TAIL))
		return;

	get_task_struct(task);

	 
	*head->lastp = node;
	head->lastp = &node->next;
}

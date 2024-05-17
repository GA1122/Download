static inline void pipelined_send(struct wake_q_head *wake_q,
				  struct mqueue_inode_info *info,
				  struct msg_msg *message,
				  struct ext_wait_queue *receiver)
{
	receiver->msg = message;
	list_del(&receiver->list);
	wake_q_add(wake_q, receiver->task);
	 
	receiver->state = STATE_READY;
}

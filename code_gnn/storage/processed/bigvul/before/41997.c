static void expunge_all(struct msg_queue *msq, int res)
{
	struct msg_receiver *msr, *t;

	list_for_each_entry_safe(msr, t, &msq->q_receivers, r_list) {
		msr->r_msg = NULL;  
		wake_up_process(msr->r_tsk);
		 
		smp_wmb();  
		msr->r_msg = ERR_PTR(res);
	}
}

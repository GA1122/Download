fst_q_work_item(u64 * queue, int card_index)
{
	unsigned long flags;
	u64 mask;

	 
	spin_lock_irqsave(&fst_work_q_lock, flags);

	 
	mask = (u64)1 << card_index;
	*queue |= mask;
	spin_unlock_irqrestore(&fst_work_q_lock, flags);
}

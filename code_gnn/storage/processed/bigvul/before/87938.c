static void fm10k_qv_enable(struct fm10k_q_vector *q_vector)
{
	 
	u32 itr = FM10K_ITR_ENABLE;

	 
	fm10k_update_itr(&q_vector->tx);

	 
	fm10k_update_itr(&q_vector->rx);

	 
	itr |= (q_vector->tx.itr & FM10K_ITR_MAX);

	 
	itr |= (q_vector->rx.itr & FM10K_ITR_MAX) << FM10K_ITR_INTERVAL1_SHIFT;

	 
	writel(itr, q_vector->itr);
}

static void seq_startplay(void)
{
	int this_one, action;
	unsigned long flags;

	while (qlen > 0)
	{

		spin_lock_irqsave(&lock,flags);
		qhead = ((this_one = qhead) + 1) % SEQ_MAX_QUEUE;
		qlen--;
		spin_unlock_irqrestore(&lock,flags);

		seq_playing = 1;

		if ((action = play_event(&queue[this_one * EV_SZ])))
		{		 
			if (action == 2)
			{
				qlen++;
				qhead = this_one;
			}
			return;
		}
	}

	seq_playing = 0;

	if ((SEQ_MAX_QUEUE - qlen) >= output_threshold)
		wake_up(&seq_sleeper);
}
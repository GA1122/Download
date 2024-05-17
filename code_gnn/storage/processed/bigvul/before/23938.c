static int RxSeqValid (struct airo_info *ai,miccntx *context,int mcast,u32 micSeq)
{
	u32 seq,index;


	if (mcast) {
		if (test_bit(FLAG_UPDATE_MULTI, &ai->flags)) {
			clear_bit (FLAG_UPDATE_MULTI, &ai->flags);
			context->window = (micSeq > 33) ? micSeq : 33;
			context->rx     = 0;         
		}
	} else if (test_bit(FLAG_UPDATE_UNI, &ai->flags)) {
		clear_bit (FLAG_UPDATE_UNI, &ai->flags);
		context->window = (micSeq > 33) ? micSeq : 33;  
		context->rx     = 0;         
	}

	seq = micSeq - (context->window - 33);

	if ((s32)seq < 0)
		return ERROR;
    
	if ( seq > 64 ) {
		MoveWindow(context,micSeq);
		return SUCCESS;
	}

	seq >>= 1;          
	index = 1 << seq;   

	if (!(context->rx & index)) {
		context->rx |= index;

		MoveWindow(context,micSeq);

		return SUCCESS;
	}
	return ERROR;
}

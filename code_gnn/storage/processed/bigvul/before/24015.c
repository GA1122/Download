static int decapsulate(struct airo_info *ai, MICBuffer *mic, etherHead *eth, u16 payLen)
{
	int      i;
	u32      micSEQ;
	miccntx  *context;
	u8       digest[4];
	mic_error micError = NONE;


	if (!ai->micstats.enabled) {
		if (memcmp ((u8*)eth + 14, micsnap, sizeof(micsnap)) == 0) {
			ai->micstats.rxMICPlummed++;
			return ERROR;
		}
		return SUCCESS;
	}

	if (ntohs(mic->typelen) == 0x888E)
		return SUCCESS;

	if (memcmp (mic->u.snap, micsnap, sizeof(micsnap)) != 0) {
		ai->micstats.rxMICPlummed++;
	    	return ERROR;
	}

	micSEQ = ntohl(mic->seq);             


	if ( (micSEQ & 1) == 0 ) {
		ai->micstats.rxWrongSequence++;
		return ERROR;
	}

	for (i = 0; i < NUM_MODULES; i++) {
		int mcast = eth->da[0] & 1;
		context = mcast ? &ai->mod[i].mCtx : &ai->mod[i].uCtx;
	
		if (!context->valid) {
			if (i == 0)
				micError = NOMICPLUMMED;
			continue;                
		}

		if (!mic->typelen)
			mic->typelen = htons(payLen + sizeof(MICBuffer) - 2);
	
		emmh32_init(&context->seed);
		emmh32_update(&context->seed, eth->da, ETH_ALEN*2); 
		emmh32_update(&context->seed, (u8 *)&mic->typelen, sizeof(mic->typelen)+sizeof(mic->u.snap)); 
		emmh32_update(&context->seed, (u8 *)&mic->seq,sizeof(mic->seq));	
		emmh32_update(&context->seed, eth->da + ETH_ALEN*2,payLen);	
		emmh32_final(&context->seed, digest);
	
		if (memcmp(digest, &mic->mic, 4)) {  
			if (i == 0)
				micError = INCORRECTMIC;
			continue;
		}

		if (RxSeqValid(ai, context, mcast, micSEQ) == SUCCESS) {
			ai->micstats.rxSuccess++;
			return SUCCESS;
		}
		if (i == 0)
			micError = SEQUENCE;
	}

	switch (micError) {
		case NOMICPLUMMED: ai->micstats.rxMICPlummed++;   break;
		case SEQUENCE:    ai->micstats.rxWrongSequence++; break;
		case INCORRECTMIC: ai->micstats.rxIncorrectMIC++; break;
		case NONE:  break;
		case NOMIC: break;
	}
	return ERROR;
}

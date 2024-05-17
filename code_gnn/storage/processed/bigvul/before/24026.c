static int encapsulate(struct airo_info *ai ,etherHead *frame, MICBuffer *mic, int payLen)
{
	miccntx   *context;


	if (test_bit(FLAG_ADHOC, &ai->flags) && (frame->da[0] & 0x1))
		context = &ai->mod[0].mCtx;
	else
		context = &ai->mod[0].uCtx;
  
	if (!context->valid)
		return ERROR;

	mic->typelen = htons(payLen + 16);  

	memcpy(&mic->u.snap, micsnap, sizeof(micsnap));  

	mic->seq = htonl(context->tx);
	context->tx += 2;

	emmh32_init(&context->seed);  
	emmh32_update(&context->seed,frame->da,ETH_ALEN * 2);  
	emmh32_update(&context->seed,(u8*)&mic->typelen,10);  
	emmh32_update(&context->seed,(u8*)&mic->seq,sizeof(mic->seq));  
	emmh32_update(&context->seed,frame->da + ETH_ALEN * 2,payLen);  
	emmh32_final(&context->seed, (u8*)&mic->mic);

	 
	mic->typelen = 0;  
	return SUCCESS;
}

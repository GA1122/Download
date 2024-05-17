static void micinit(struct airo_info *ai)
{
	MICRid mic_rid;

	clear_bit(JOB_MIC, &ai->jobs);
	PC4500_readrid(ai, RID_MIC, &mic_rid, sizeof(mic_rid), 0);
	up(&ai->sem);

	ai->micstats.enabled = (le16_to_cpu(mic_rid.state) & 0x00FF) ? 1 : 0;
	if (!ai->micstats.enabled) {
		 
		ai->mod[0].uCtx.valid = 0;
		ai->mod[0].mCtx.valid = 0;
		return;
	}

	if (mic_rid.multicastValid) {
		age_mic_context(&ai->mod[0].mCtx, &ai->mod[1].mCtx,
		                mic_rid.multicast, sizeof(mic_rid.multicast),
		                ai->tfm);
	}

	if (mic_rid.unicastValid) {
		age_mic_context(&ai->mod[0].uCtx, &ai->mod[1].uCtx,
				mic_rid.unicast, sizeof(mic_rid.unicast),
				ai->tfm);
	}
}
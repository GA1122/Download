check_slot_seqid(u32 seqid, u32 slot_seqid, int slot_inuse)
{
	dprintk("%s enter. seqid %d slot_seqid %d\n", __func__, seqid,
		slot_seqid);

	 
	if (slot_inuse) {
		if (seqid == slot_seqid)
			return nfserr_jukebox;
		else
			return nfserr_seq_misordered;
	}
	 
	if (likely(seqid == slot_seqid + 1))
		return nfs_ok;
	if (seqid == slot_seqid)
		return nfserr_replay_cache;
	return nfserr_seq_misordered;
}
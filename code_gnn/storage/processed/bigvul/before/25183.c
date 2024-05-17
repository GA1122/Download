static u32 rt_peer_genid(void)
{
	return atomic_read(&__rt_peer_genid);
}

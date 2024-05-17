void snd_seq_autoload_init(void)
{
	atomic_dec(&snd_seq_in_init);
#ifdef CONFIG_SND_SEQUENCER_MODULE
	 
	queue_autoload_drivers();
#endif
}

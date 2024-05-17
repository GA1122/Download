static inline bool rb_reader_lock(struct ring_buffer_per_cpu *cpu_buffer)
{
	if (likely(!in_nmi())) {
		raw_spin_lock(&cpu_buffer->reader_lock);
		return true;
	}

	 
	if (raw_spin_trylock(&cpu_buffer->reader_lock))
		return true;

	 
	atomic_inc(&cpu_buffer->record_disabled);
	return false;
}

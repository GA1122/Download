static u64 efx_get_atomic_stat(void *field)
{
	return atomic_read((atomic_t *) field);
}

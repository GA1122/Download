get_entry(const void *base, unsigned int offset)
{
	return (struct ip6t_entry *)(base + offset);
}
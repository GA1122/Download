get_entry(const void *base, unsigned int offset)
{
	return (struct ipt_entry *)(base + offset);
}
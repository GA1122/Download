static ssize_t map_addr_show(struct uio_mem *mem, char *buf)
{
	return sprintf(buf, "0x%llx\n", (unsigned long long)mem->addr);
}
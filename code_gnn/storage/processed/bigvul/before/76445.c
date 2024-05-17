static inline void msg_init(struct uffd_msg *msg)
{
	BUILD_BUG_ON(sizeof(struct uffd_msg) != 32);
	 
	memset(msg, 0, sizeof(struct uffd_msg));
}

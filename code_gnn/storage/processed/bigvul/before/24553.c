static u16 generate_cookie(struct b43_dmaring *ring, int slot)
{
	u16 cookie;

	 
	cookie = (((u16)ring->index + 1) << 12);
	B43_WARN_ON(slot & ~0x0FFF);
	cookie |= (u16)slot;

	return cookie;
}

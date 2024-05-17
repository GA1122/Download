static inline int vring_need_event(uint16_t event, uint16_t new, uint16_t old)
{
	 
	return (uint16_t)(new - event - 1) < (uint16_t)(new - old);
}

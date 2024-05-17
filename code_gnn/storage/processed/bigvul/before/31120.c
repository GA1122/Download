static bool apertures_overlap(struct aperture *gen, struct aperture *hw)
{
	 
	if (gen->base == hw->base)
		return true;
	 
	if (gen->base > hw->base && gen->base < hw->base + hw->size)
		return true;
	return false;
}

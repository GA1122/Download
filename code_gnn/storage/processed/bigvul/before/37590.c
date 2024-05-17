static bool spte_has_volatile_bits(u64 spte)
{
	 
	if (spte_is_locklessly_modifiable(spte))
		return true;

	if (!shadow_accessed_mask)
		return false;

	if (!is_shadow_present_pte(spte))
		return false;

	if ((spte & shadow_accessed_mask) &&
	      (!is_writable_pte(spte) || (spte & shadow_dirty_mask)))
		return false;

	return true;
}

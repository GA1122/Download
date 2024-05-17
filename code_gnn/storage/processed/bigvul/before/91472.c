static bool regsafe(struct bpf_reg_state *rold, struct bpf_reg_state *rcur,
		    struct idpair *idmap)
{
	bool equal;

	if (!(rold->live & REG_LIVE_READ))
		 
		return true;

	equal = memcmp(rold, rcur, offsetof(struct bpf_reg_state, parent)) == 0;

	if (rold->type == PTR_TO_STACK)
		 
		return equal && rold->frameno == rcur->frameno;

	if (equal)
		return true;

	if (rold->type == NOT_INIT)
		 
		return true;
	if (rcur->type == NOT_INIT)
		return false;
	switch (rold->type) {
	case SCALAR_VALUE:
		if (rcur->type == SCALAR_VALUE) {
			 
			return range_within(rold, rcur) &&
			       tnum_in(rold->var_off, rcur->var_off);
		} else {
			 
			return false;
		}
	case PTR_TO_MAP_VALUE:
		 
		return memcmp(rold, rcur, offsetof(struct bpf_reg_state, id)) == 0 &&
		       range_within(rold, rcur) &&
		       tnum_in(rold->var_off, rcur->var_off);
	case PTR_TO_MAP_VALUE_OR_NULL:
		 
		if (rcur->type != PTR_TO_MAP_VALUE_OR_NULL)
			return false;
		if (memcmp(rold, rcur, offsetof(struct bpf_reg_state, id)))
			return false;
		 
		return check_ids(rold->id, rcur->id, idmap);
	case PTR_TO_PACKET_META:
	case PTR_TO_PACKET:
		if (rcur->type != rold->type)
			return false;
		 
		if (rold->range > rcur->range)
			return false;
		 
		if (rold->off != rcur->off)
			return false;
		 
		if (rold->id && !check_ids(rold->id, rcur->id, idmap))
			return false;
		 
		return range_within(rold, rcur) &&
		       tnum_in(rold->var_off, rcur->var_off);
	case PTR_TO_CTX:
	case CONST_PTR_TO_MAP:
	case PTR_TO_PACKET_END:
	case PTR_TO_FLOW_KEYS:
	case PTR_TO_SOCKET:
	case PTR_TO_SOCKET_OR_NULL:
		 
	default:
		 
		return false;
	}

	 
	WARN_ON_ONCE(1);
	return false;
}

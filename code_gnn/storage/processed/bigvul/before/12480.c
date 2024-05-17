void check_defer(int nid)
	{
	if (!obj_cleanup_defer && nid >= NUM_NID)
			obj_cleanup_defer = 1;
	}

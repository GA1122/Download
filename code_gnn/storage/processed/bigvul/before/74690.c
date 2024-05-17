static inline int check_ext_ctrls(struct v4l2_ext_controls *c, int allow_priv)
{
	__u32 i;

	 
	c->reserved[0] = c->reserved[1] = 0;
	for (i = 0; i < c->count; i++)
		c->controls[i].reserved2[0] = 0;

	 
	if (!allow_priv && c->ctrl_class == V4L2_CID_PRIVATE_BASE)
		return 0;
	 
	for (i = 0; i < c->count; i++) {
		if (V4L2_CTRL_ID2CLASS(c->controls[i].id) != c->ctrl_class) {
			c->error_idx = i;
			return 0;
		}
	}
	return 1;
}

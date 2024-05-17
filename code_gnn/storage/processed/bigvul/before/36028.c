 static int ceph_x_is_authenticated(struct ceph_auth_client *ac)
{
	struct ceph_x_info *xi = ac->private;
	int need;

	ceph_x_validate_tickets(ac, &need);
	dout("ceph_x_is_authenticated want=%d need=%d have=%d\n",
	     ac->want_keys, need, xi->have_keys);
	return (ac->want_keys & xi->have_keys) == ac->want_keys;
}
static int h2_init(struct connection *conn)
{
	if (conn->mux_ctx) {
		 
		return -1;
	}

	return h2c_frt_init(conn);
}

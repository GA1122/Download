set_new_values(int disc_state, double offset, double recv_time)
{
	 
	VERB4 bb_error_msg("disc_state=%d last update offset=%f recv_time=%f",
			disc_state, offset, recv_time);
	G.discipline_state = disc_state;
	G.last_update_offset = offset;
	G.last_update_recv_time = recv_time;
}
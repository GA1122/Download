static void stroke_user_creds(private_stroke_socket_t *this,
							  stroke_msg_t *msg, FILE *out)
{
	pop_string(msg, &msg->user_creds.name);
	pop_string(msg, &msg->user_creds.username);
	pop_string(msg, &msg->user_creds.password);

	DBG1(DBG_CFG, "received stroke: user-creds '%s'", msg->user_creds.name);

	this->config->set_user_credentials(this->config, msg, out);
}

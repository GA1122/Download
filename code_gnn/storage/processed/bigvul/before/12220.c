static void stroke_rekey(private_stroke_socket_t *this, stroke_msg_t *msg, FILE *out)
{
	pop_string(msg, &msg->terminate.name);
	DBG1(DBG_CFG, "received stroke: rekey '%s'", msg->rekey.name);

	this->control->rekey(this->control, msg, out);
}

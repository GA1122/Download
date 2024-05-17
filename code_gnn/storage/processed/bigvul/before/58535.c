static BOOL freerdp_peer_close(freerdp_peer* client)
{
	 
	if (!rdp_send_deactivate_all(client->context->rdp))
		return FALSE;
	return mcs_send_disconnect_provider_ultimatum(client->context->rdp->mcs);
}

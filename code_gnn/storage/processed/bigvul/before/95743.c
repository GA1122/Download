void CL_UpdateVoipIgnore(const char *idstr, qboolean ignore)
{
	if ((*idstr >= '0') && (*idstr <= '9')) {
		const int id = atoi(idstr);
		if ((id >= 0) && (id < MAX_CLIENTS)) {
			clc.voipIgnore[id] = ignore;
			CL_AddReliableCommand(va("voip %s %d",
			                         ignore ? "ignore" : "unignore", id), qfalse);
			Com_Printf("VoIP: %s ignoring player #%d\n",
			            ignore ? "Now" : "No longer", id);
			return;
		}
	}
	Com_Printf("VoIP: invalid player ID#\n");
}

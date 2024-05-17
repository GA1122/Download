void CL_UpdateVoipGain(const char *idstr, float gain)
{
	if ((*idstr >= '0') && (*idstr <= '9')) {
		const int id = atoi(idstr);
		if (gain < 0.0f)
			gain = 0.0f;
		if ((id >= 0) && (id < MAX_CLIENTS)) {
			clc.voipGain[id] = gain;
			Com_Printf("VoIP: player #%d gain now set to %f\n", id, gain);
		}
	}
}

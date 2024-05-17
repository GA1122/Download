void CL_VoipNewGeneration(void)
{
	clc.voipOutgoingGeneration++;
	if (clc.voipOutgoingGeneration <= 0)
		clc.voipOutgoingGeneration = 1;
	clc.voipPower = 0.0f;
	clc.voipOutgoingSequence = 0;

	opus_encoder_ctl(clc.opusEncoder, OPUS_RESET_STATE);
}

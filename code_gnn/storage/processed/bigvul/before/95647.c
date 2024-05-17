void CL_CaptureVoip(void)
{
	const float audioMult = cl_voipCaptureMult->value;
	const qboolean useVad = (cl_voipUseVAD->integer != 0);
	qboolean initialFrame = qfalse;
	qboolean finalFrame = qfalse;

#if USE_MUMBLE
	if (cl_useMumble->integer)
		return;
#endif

	if (cl_voip->modified || cl_rate->modified) {
		if ((cl_voip->integer) && (cl_rate->integer < 25000)) {
			Com_Printf(S_COLOR_YELLOW "Your network rate is too slow for VoIP.\n");
			Com_Printf("Set 'Data Rate' to 'LAN/Cable/xDSL' in 'Setup/System/Network'.\n");
			Com_Printf("Until then, VoIP is disabled.\n");
			Cvar_Set("cl_voip", "0");
		}
		Cvar_Set("cl_voipProtocol", cl_voip->integer ? "opus" : "");
		cl_voip->modified = qfalse;
		cl_rate->modified = qfalse;
	}

	if (!clc.voipCodecInitialized)
		return;   

	if (clc.voipOutgoingDataSize > 0)
		return;   

	if (cl_voipUseVAD->modified) {
		Cvar_Set("cl_voipSend", (useVad) ? "1" : "0");
		cl_voipUseVAD->modified = qfalse;
	}

	if ((useVad) && (!cl_voipSend->integer))
		Cvar_Set("cl_voipSend", "1");   

	if (cl_voipSend->modified) {
		qboolean dontCapture = qfalse;
		if (clc.state != CA_ACTIVE)
			dontCapture = qtrue;   
		else if (!clc.voipEnabled)
			dontCapture = qtrue;   
		else if (clc.demoplaying)
			dontCapture = qtrue;   
		else if ( cl_voip->integer == 0 )
			dontCapture = qtrue;   
		else if ( audioMult == 0.0f )
			dontCapture = qtrue;   

		cl_voipSend->modified = qfalse;

		if(dontCapture)
		{
			Cvar_Set("cl_voipSend", "0");
			return;
		}

		if (cl_voipSend->integer) {
			initialFrame = qtrue;
		} else {
			finalFrame = qtrue;
		}
	}


	if (initialFrame) {
		S_MasterGain(Com_Clamp(0.0f, 1.0f, cl_voipGainDuringCapture->value));
		S_StartCapture();
		CL_VoipNewGeneration();
		CL_VoipParseTargets();
	}

	if ((cl_voipSend->integer) || (finalFrame)) {  
		int samples = S_AvailableCaptureSamples();
		const int packetSamples = (finalFrame) ? VOIP_MAX_FRAME_SAMPLES : VOIP_MAX_PACKET_SAMPLES;

		if (samples >= packetSamples) {
			static int16_t sampbuffer[VOIP_MAX_PACKET_SAMPLES];
			float voipPower = 0.0f;
			int voipFrames;
			int i, bytes;

			if (samples > VOIP_MAX_PACKET_SAMPLES)
				samples = VOIP_MAX_PACKET_SAMPLES;


			samples -= samples % VOIP_MAX_FRAME_SAMPLES;
			if (samples != 120 && samples != 240 && samples != 480 && samples != 960 && samples != 1920 && samples != 2880 ) {
				Com_Printf("Voip: bad number of samples %d\n", samples);
				return;
			}
			voipFrames = samples / VOIP_MAX_FRAME_SAMPLES;

			S_Capture(samples, (byte *) sampbuffer);   

			for (i = 0; i < samples; i++) {
				const float flsamp = (float) sampbuffer[i];
				const float s = fabs(flsamp);
				voipPower += s * s;
				sampbuffer[i] = (int16_t) ((flsamp) * audioMult);
			}

			bytes = opus_encode(clc.opusEncoder, sampbuffer, samples,
									(unsigned char *) clc.voipOutgoingData,
									sizeof (clc.voipOutgoingData));
			if ( bytes <= 0 ) {
				Com_DPrintf("VoIP: Error encoding %d samples\n", samples);
				bytes = 0;
			}

			clc.voipPower = (voipPower / (32768.0f * 32768.0f *
			                 ((float) samples))) * 100.0f;

			if ((useVad) && (clc.voipPower < cl_voipVADThreshold->value)) {
				CL_VoipNewGeneration();   
			} else {
				clc.voipOutgoingDataSize = bytes;
				clc.voipOutgoingDataFrames = voipFrames;

				Com_DPrintf("VoIP: Send %d frames, %d bytes, %f power\n",
				            voipFrames, bytes, clc.voipPower);

				#if 0
				static FILE *encio = NULL;
				if (encio == NULL) encio = fopen("voip-outgoing-encoded.bin", "wb");
				if (encio != NULL) { fwrite(clc.voipOutgoingData, bytes, 1, encio); fflush(encio); }
				static FILE *decio = NULL;
				if (decio == NULL) decio = fopen("voip-outgoing-decoded.bin", "wb");
				if (decio != NULL) { fwrite(sampbuffer, voipFrames * VOIP_MAX_FRAME_SAMPLES * 2, 1, decio); fflush(decio); }
				#endif
			}
		}
	}

	if (finalFrame) {
		S_StopCapture();
		S_MasterGain(1.0f);
		clc.voipPower = 0.0f;   
	}
}

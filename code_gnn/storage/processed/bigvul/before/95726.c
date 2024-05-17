void CL_ShutdownAll(qboolean shutdownRef)
{
	if(CL_VideoRecording())
		CL_CloseAVI();

	if(clc.demorecording)
		CL_StopRecord_f();

#ifdef USE_CURL
	CL_cURL_Shutdown();
#endif
	S_DisableSounds();
	CL_ShutdownCGame();
	CL_ShutdownUI();

	if(shutdownRef)
		CL_ShutdownRef();
	else if(re.Shutdown)
		re.Shutdown(qfalse);		 

	cls.uiStarted = qfalse;
	cls.cgameStarted = qfalse;
	cls.rendererStarted = qfalse;
	cls.soundRegistered = qfalse;
}

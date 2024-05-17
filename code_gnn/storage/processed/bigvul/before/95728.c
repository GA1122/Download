void CL_Snd_Restart_f(void)
{
	CL_Snd_Shutdown();
	CL_Vid_Restart_f();
}

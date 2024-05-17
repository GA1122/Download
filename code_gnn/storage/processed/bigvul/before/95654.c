void CL_ClearMemory(qboolean shutdownRef)
{
	CL_ShutdownAll(shutdownRef);

	if ( !com_sv_running->integer ) {
		Hunk_Clear();
		CM_ClearMap();
	} else {
		Hunk_ClearToMark();
	}
}

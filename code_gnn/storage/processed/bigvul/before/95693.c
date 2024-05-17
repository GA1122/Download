static void CL_OldGame(void)
{
	if(cl_oldGameSet)
	{
		cl_oldGameSet = qfalse;
		Cvar_Set2("fs_game", cl_oldGame, qtrue);
		FS_ConditionalRestart(clc.checksumFeed, qfalse);
	}
}

void CL_SendPureChecksums( void ) {
	char cMsg[MAX_INFO_VALUE];

	Com_sprintf(cMsg, sizeof(cMsg), "cp %d %s", cl.serverId, FS_ReferencedPakPureChecksums());

	CL_AddReliableCommand(cMsg, qfalse);
}

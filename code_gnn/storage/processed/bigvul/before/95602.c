void Com_InitPushEvent( void ) {
	memset( com_pushedEvents, 0, sizeof( com_pushedEvents ) );
	com_pushedEventsHead = 0;
	com_pushedEventsTail = 0;
}

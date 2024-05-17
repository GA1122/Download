EventHandler::display()
{
	dprintf( D_ALWAYS, "EventHandler {\n" );

	dprintf( D_ALWAYS, "\tfunc = %p\n", func );
	display_sigset( "\tmask = ", &mask );

	dprintf( D_ALWAYS, "}\n" );
}

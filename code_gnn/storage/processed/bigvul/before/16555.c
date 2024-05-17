EventHandler::EventHandler( void (*f)(int), sigset_t m )
{
	func = f;
	mask = m;
	is_installed = FALSE;
}

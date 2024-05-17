intTrap(SIGNAL_ARG)
{				 
    LONGJMP(IntReturn, 0);
    SIGNAL_RETURN;
}

dfaerror (char const *mesg)
{
  error (EXIT_TROUBLE, 0, "%s", mesg);

   
   
  abort ();
}

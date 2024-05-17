void _exit(int status) {
  fprint_str(stderr, "_exit\n");
   
  *(int *) 1 = status;
  while (1)
    ;  
}

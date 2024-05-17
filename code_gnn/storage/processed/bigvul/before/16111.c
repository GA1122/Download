isSuperUser( const char* user )
{
  if( ! (user)) {
    return false;
  }

  super_users.rewind();
  char * next;
  while ((next = super_users.next())) {
    if (strcmp (user, next ) == 0) {
      return true;
    }
  }

  return false;
}

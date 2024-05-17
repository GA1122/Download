const char *npw_strerror(int error)
{
  if (error > -1100 && error <= -1000)	 
	return rpc_strerror(error);

  switch (error) {
  case 0:	return "No error";
  }

  return "Unknown error";
}

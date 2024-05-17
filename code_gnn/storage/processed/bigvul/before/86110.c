is_path_absolute(const char *path)	 
{
  

  if (path[0] != '/')
    return (0);

  

  if (strchr(path, '<') != NULL || strchr(path, '\"') != NULL || strchr(path, '\'') != NULL)
    return (0);

  

  while ((path = strstr(path, "/..")) != NULL)
  {
    if (!path[3] || path[3] == '/')
      return (0);

    path ++;
  }

  

  return (1);
}

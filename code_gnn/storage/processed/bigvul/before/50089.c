static char* dup_nickname(struct Curl_easy *data, enum dupstring cert_kind)
{
  const char *str = data->set.str[cert_kind];
  const char *n;

  if(!is_file(str))
     
    return strdup(str);

   
  n = strchr(str, '/');
  if(!n) {
    infof(data, "warning: certificate file name \"%s\" handled as nickname; "
          "please use \"./%s\" to force file name\n", str, str);
    return strdup(str);
  }

   
  return NULL;
}

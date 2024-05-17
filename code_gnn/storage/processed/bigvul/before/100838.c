bool HttpUtil::IsLWS(char c) {
  return strchr(HTTP_LWS, c) != NULL;
}

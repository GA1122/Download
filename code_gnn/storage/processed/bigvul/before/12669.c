prvs_daystamp(int day_offset)
{
uschar *days = store_get(32);                 
(void)string_format(days, 32, TIME_T_FMT,     
  (time(NULL) + day_offset*86400)/86400);
return (Ustrlen(days) >= 3) ? &days[Ustrlen(days)-3] : US"100";
}

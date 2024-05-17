void setup_test_env(void)
{
 int i = 0;

 while (console_cmd_list[i].name != NULL)
 {
        console_cmd_maxlen = MAX(console_cmd_maxlen, (int)strlen(console_cmd_list[i].name));
        i++;
 }
}

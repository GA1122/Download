void do_help(char UNUSED *p)
{
 int i = 0;
 int max = 0;
 char line[128];
 int pos = 0;

 while (console_cmd_list[i].name != NULL)
 {
        pos = sprintf(line, "%s", (char*)console_cmd_list[i].name);
        bdt_log("%s %s\n", (char*)line, (char*)console_cmd_list[i].help);
        i++;
 }
}

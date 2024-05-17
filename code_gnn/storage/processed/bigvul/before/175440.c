static int get_command_status(int status, int fct_status, uint32_t cmd_status) {
 if (fct_status != 0)
        status = fct_status;
 else if (cmd_status != 0)
        status = cmd_status;
 return status;
}

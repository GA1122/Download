static bool cmd_check_power_mode(IDEState *s, uint8_t cmd)
{
    s->nsector = 0xff;  
    return true;
}

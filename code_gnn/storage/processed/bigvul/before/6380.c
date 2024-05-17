set_ctrl(E1000State *s, int index, uint32_t val)
{
     
    s->mac_reg[CTRL] = val & ~E1000_CTRL_RST;
}

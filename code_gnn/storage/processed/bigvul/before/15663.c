static uint32_t ssd0323_transfer(SSISlave *dev, uint32_t data)
{
    ssd0323_state *s = FROM_SSI_SLAVE(ssd0323_state, dev);

    switch (s->mode) {
    case SSD0323_DATA:
        DPRINTF("data 0x%02x\n", data);
        s->framebuffer[s->col + s->row * 64] = data;
        if (s->remap & REMAP_VERTICAL) {
            s->row++;
            if (s->row > s->row_end) {
                s->row = s->row_start;
                s->col++;
            }
            if (s->col > s->col_end) {
                s->col = s->col_start;
            }
        } else {
            s->col++;
            if (s->col > s->col_end) {
                s->row++;
                s->col = s->col_start;
            }
            if (s->row > s->row_end) {
                s->row = s->row_start;
            }
        }
        s->redraw = 1;
        break;
    case SSD0323_CMD:
        DPRINTF("cmd 0x%02x\n", data);
        if (s->cmd_len == 0) {
            s->cmd = data;
        } else {
            s->cmd_data[s->cmd_len - 1] = data;
        }
        s->cmd_len++;
        switch (s->cmd) {
#define DATA(x) if (s->cmd_len <= (x)) return 0
        case 0x15:  
            DATA(2);
            s->col = s->col_start = s->cmd_data[0] % 64;
            s->col_end = s->cmd_data[1] % 64;
            break;
        case 0x75:  
            DATA(2);
            s->row = s->row_start = s->cmd_data[0] % 80;
            s->row_end = s->cmd_data[1] % 80;
            break;
        case 0x81:  
            DATA(1);
            break;
        case 0x84: case 0x85: case 0x86:  
            DATA(0);
            break;
        case 0xa0:  
             
            DATA(1);
            s->remap = s->cmd_data[0];
            break;
        case 0xa1:  
        case 0xa2:  
             
            DATA(1);
            break;
        case 0xa4:  
        case 0xa5:  
        case 0xa6:  
        case 0xa7:  
             
            DATA(0);
            break;
        case 0xa8:  
        case 0xad:  
            DATA(1);
             
            break;
        case 0xae:  
        case 0xaf:  
            DATA(0);
             
            break;
        case 0xb1:  
        case 0xb2:  
        case 0xb3:  
        case 0xbc:  
        case 0xbe:  
        case 0xbf:  
            DATA(1);
             
            break;
        case 0xb8:  
             
            DATA(8);
            break;
        case 0xe3:  
            DATA(0);
            break;
        case 0xff:  
            break;
        default:
            BADF("Unknown command: 0x%x\n", data);
        }
        s->cmd_len = 0;
        return 0;
    }
    return 0;
}

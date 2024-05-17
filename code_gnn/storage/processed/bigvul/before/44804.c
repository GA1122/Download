local void list_info(void)
{
    int method;              
    size_t n;                
    off_t at;                
    unsigned char tail[8];   
    unsigned long check, len;    

     
    in_init();

     
    method = get_header(1);
    if (method < 0) {
        RELEASE(g.hname);
        if (method != -1 && g.verbosity > 1)
            complain("%s not a compressed file -- skipping", g.inf);
        return;
    }

     
    if (g.form > 1) {
        g.in_tot = g.zip_clen;
        show_info(method, g.zip_crc, g.zip_ulen, 0);
        return;
    }

     
    if (g.form == 1) {
        at = lseek(g.ind, 0, SEEK_END);
        if (at == -1) {
            check = 0;
            do {
                len = g.in_left < 4 ? g.in_left : 4;
                g.in_next += g.in_left - len;
                while (len--)
                    check = (check << 8) + *g.in_next++;
            } while (load() != 0);
            check &= LOW32;
        }
        else {
            g.in_tot = at;
            lseek(g.ind, -4, SEEK_END);
            readn(g.ind, tail, 4);
            check = PULL4M(tail);
        }
        g.in_tot -= 6;
        show_info(method, check, 0, 0);
        return;
    }

     
    if (method == 257) {
        at = lseek(g.ind, 0, SEEK_END);
        if (at == -1)
            while (load() != 0)
                ;
        else
            g.in_tot = at;
        g.in_tot -= 3;
        show_info(method, 0, 0, 0);
        return;
    }

     
    if (g.in_short) {                    
        if (g.in_left < 8) {
            complain("%s not a valid gzip file -- skipping", g.inf);
            return;
        }
        g.in_tot = g.in_left - 8;        
        memcpy(tail, g.in_next + (g.in_left - 8), 8);
    }
    else if ((at = lseek(g.ind, -8, SEEK_END)) != -1) {
        g.in_tot = at - g.in_tot + g.in_left;    
        readn(g.ind, tail, 8);           
    }
    else {                               
        at = g.in_tot - g.in_left;       
        do {
            n = g.in_left < 8 ? g.in_left : 8;
            memcpy(tail, g.in_next + (g.in_left - n), n);
            load();
        } while (g.in_left == BUF);      
        if (g.in_left < 8) {
            if (n + g.in_left < 8) {
                complain("%s not a valid gzip file -- skipping", g.inf);
                return;
            }
            if (g.in_left) {
                if (n + g.in_left > 8)
                    memcpy(tail, tail + n - (8 - g.in_left), 8 - g.in_left);
                memcpy(tail + 8 - g.in_left, g.in_next, g.in_left);
            }
        }
        else
            memcpy(tail, g.in_next + (g.in_left - 8), 8);
        g.in_tot -= at + 8;
    }
    if (g.in_tot < 2) {
        complain("%s not a valid gzip file -- skipping", g.inf);
        return;
    }

     
    check = PULL4L(tail);
    len = PULL4L(tail + 4);

     
    show_info(method, check, len, 0);
    RELEASE(g.hname);
}

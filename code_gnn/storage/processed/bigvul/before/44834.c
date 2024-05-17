local void unlzw(void)
{
    unsigned bits;               
    unsigned mask;               
    bits_t buf;                  
    unsigned left;               
    off_t mark;                  
    unsigned code;               
    unsigned max;                
    unsigned flags;              
    unsigned end;                
    unsigned prev;               
    unsigned final;              
    unsigned stack;              
    unsigned outcnt;             
     
    uint_least16_t prefix[65536];        
    unsigned char suffix[65536];         
    unsigned char match[65280 + 2];      

     
    g.out_tot = 0;
    if (NOMORE())
        bail("lzw premature end: ", g.inf);
    flags = NEXT();
    if (flags & 0x60)
        bail("unknown lzw flags set: ", g.inf);
    max = flags & 0x1f;
    if (max < 9 || max > 16)
        bail("lzw bits out of range: ", g.inf);
    if (max == 9)                            
        max = 10;
    flags &= 0x80;                           

     
    mark = g.in_tot - g.in_left;

     
    bits = 9;
    mask = 0x1ff;
    end = flags ? 256 : 255;

     
    if (NOMORE())                            
        return;
    buf = NEXT();
    if (NOMORE())
        bail("lzw premature end: ", g.inf);  
    buf += NEXT() << 8;
    final = prev = buf & mask;               
    buf >>= bits;
    left = 16 - bits;
    if (prev > 255)
        bail("invalid lzw code: ", g.inf);
    out_buf[0] = final;                      
    outcnt = 1;

     
    stack = 0;
    for (;;) {
         
        if (end >= mask && bits < max) {
             
            {
                unsigned rem = ((g.in_tot - g.in_left) - mark) % bits;
                if (rem)
                    rem = bits - rem;
                while (rem > g.in_left) {
                    rem -= g.in_left;
                    if (load() == 0)
                        break;
                }
                g.in_left -= rem;
                g.in_next += rem;
            }
            buf = 0;
            left = 0;

             
            mark = g.in_tot - g.in_left;

             
            bits++;
            mask <<= 1;
            mask++;
        }

         
        if (NOMORE())
            break;                           
        buf += (bits_t)(NEXT()) << left;
        left += 8;
        if (left < bits) {
            if (NOMORE())
                bail("lzw premature end: ", g.inf);
            buf += (bits_t)(NEXT()) << left;
            left += 8;
        }
        code = buf & mask;
        buf >>= bits;
        left -= bits;

         
        if (code == 256 && flags) {
             
            {
                unsigned rem = ((g.in_tot - g.in_left) - mark) % bits;
                if (rem)
                    rem = bits - rem;
                while (rem > g.in_left) {
                    rem -= g.in_left;
                    if (load() == 0)
                        break;
                }
                g.in_left -= rem;
                g.in_next += rem;
            }
            buf = 0;
            left = 0;

             
            mark = g.in_tot - g.in_left;

             
            bits = 9;                        
            mask = 0x1ff;
            end = 255;                       
            continue;                        
        }

         
        {
            unsigned temp = code;            
            if (code > end) {
                 
                if (code != end + 1 || prev > end)
                    bail("invalid lzw code: ", g.inf);
                match[stack++] = final;
                code = prev;
            }

             
            while (code >= 256) {
                match[stack++] = suffix[code];
                code = prefix[code];
            }
            match[stack++] = code;
            final = code;

             
            if (end < mask) {
                end++;
                prefix[end] = prev;
                suffix[end] = final;
            }

             
            prev = temp;
        }

         
        while (stack > OUTSIZE - outcnt) {
            while (outcnt < OUTSIZE)
                out_buf[outcnt++] = match[--stack];
            g.out_tot += outcnt;
            if (g.decode == 1)
                writen(g.outd, out_buf, outcnt);
            outcnt = 0;
        }
        do {
            out_buf[outcnt++] = match[--stack];
        } while (stack);
    }

     
    g.out_tot += outcnt;
    if (outcnt && g.decode == 1)
        writen(g.outd, out_buf, outcnt);
}

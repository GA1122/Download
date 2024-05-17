local void put_trailer(unsigned long ulen, unsigned long clen,
                       unsigned long check, unsigned long head)
{
    unsigned char tail[46];

    if (g.form > 1) {                
        unsigned long cent;

         
        PUT4L(tail, 0x08074b50UL);
        PUT4L(tail + 4, check);
        PUT4L(tail + 8, clen);
        PUT4L(tail + 12, ulen);
        writen(g.outd, tail, 16);

         
        PUT4L(tail, 0x02014b50UL);   
        tail[4] = 63;                
        tail[5] = 255;               
        PUT2L(tail + 6, 20);         
        PUT2L(tail + 8, 8);          
        PUT2L(tail + 10, 8);         
        PUT4L(tail + 12, time2dos(g.mtime));
        PUT4L(tail + 16, check);     
        PUT4L(tail + 20, clen);      
        PUT4L(tail + 24, ulen);      
        PUT2L(tail + 28, g.name == NULL ? 1 :    
                                          strlen(g.name));
        PUT2L(tail + 30, 9);         
        PUT2L(tail + 32, 0);         
        PUT2L(tail + 34, 0);         
        PUT2L(tail + 36, 0);         
        PUT4L(tail + 38, 0);         
        PUT4L(tail + 42, 0);         
        writen(g.outd, tail, 46);    
        cent = 46;

         
        if (g.name == NULL)
            writen(g.outd, (unsigned char *)"-", 1);
        else
            writen(g.outd, (unsigned char *)g.name, strlen(g.name));
        cent += g.name == NULL ? 1 : strlen(g.name);

         
        PUT2L(tail, 0x5455);         
        PUT2L(tail + 2, 5);          
        tail[4] = 1;                 
        PUT4L(tail + 5, g.mtime);    
        writen(g.outd, tail, 9);     
        cent += 9;

         
        PUT4L(tail, 0x06054b50UL);   
        PUT2L(tail + 4, 0);          
        PUT2L(tail + 6, 0);          
        PUT2L(tail + 8, 1);          
        PUT2L(tail + 10, 1);         
        PUT4L(tail + 12, cent);      
        PUT4L(tail + 16, head + clen + 16);  
        PUT2L(tail + 20, 0);         
        writen(g.outd, tail, 22);    
    }
    else if (g.form) {               
        PUT4M(tail, check);
        writen(g.outd, tail, 4);
    }
    else {                           
        PUT4L(tail, check);
        PUT4L(tail + 4, ulen);
        writen(g.outd, tail, 8);
    }
}

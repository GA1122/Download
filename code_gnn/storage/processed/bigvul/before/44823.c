local unsigned long put_header(void)
{
    unsigned long len;
    unsigned char head[30];

    if (g.form > 1) {                
         
        PUT4L(head, 0x04034b50UL);   
        PUT2L(head + 4, 20);         
        PUT2L(head + 6, 8);          
        PUT2L(head + 8, 8);          
        PUT4L(head + 10, time2dos(g.mtime));
        PUT4L(head + 14, 0);         
        PUT4L(head + 18, 0);         
        PUT4L(head + 22, 0);         
        PUT2L(head + 26, g.name == NULL ? 1 :    
                                          strlen(g.name));
        PUT2L(head + 28, 9);         
        writen(g.outd, head, 30);    
        len = 30;

         
        if (g.name == NULL)
            writen(g.outd, (unsigned char *)"-", 1);
        else
            writen(g.outd, (unsigned char *)g.name, strlen(g.name));
        len += g.name == NULL ? 1 : strlen(g.name);

         
        PUT2L(head, 0x5455);         
        PUT2L(head + 2, 5);          
        head[4] = 1;                 
        PUT4L(head + 5, g.mtime);    
        writen(g.outd, head, 9);     
        len += 9;
    }
    else if (g.form) {               
        head[0] = 0x78;              
        head[1] = (g.level >= 9 ? 3 :
                   (g.level == 1 ? 0 :
                    (g.level >= 6 || g.level == Z_DEFAULT_COMPRESSION ?
                        1 : 2))) << 6;
        head[1] += 31 - (((head[0] << 8) + head[1]) % 31);
        writen(g.outd, head, 2);
        len = 2;
    }
    else {                           
        head[0] = 31;
        head[1] = 139;
        head[2] = 8;                 
        head[3] = g.name != NULL ? 8 : 0;
        PUT4L(head + 4, g.mtime);
        head[8] = g.level >= 9 ? 2 : (g.level == 1 ? 4 : 0);
        head[9] = 3;                 
        writen(g.outd, head, 10);
        len = 10;
        if (g.name != NULL)
            writen(g.outd, (unsigned char *)g.name, strlen(g.name) + 1);
        if (g.name != NULL)
            len += strlen(g.name) + 1;
    }
    return len;
}

local void cat(void)
{
     
    writen(g.outd, &g.magic1, 1);
    g.out_tot = 1;

     
    while (g.in_left) {
        writen(g.outd, g.in_next, g.in_left);
        g.out_tot += g.in_left;
        g.in_left = 0;
        load();
    }
}

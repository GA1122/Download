print_remote_ip (request * req, FILE *fp)
{
    if (log_forwarded_for) {
        const char *s = req->header_forwarded_for;
        if (s && *s) {
            for (; *s; s++) {
               
                if (strchr ("0123456789.abcdef:ABCDEF,", *s))
                  putc (*s, fp);
                else
                  putc ('_', fp);
            }
        }
        else  
          fprintf (fp, "(%s)", req->remote_ip_addr);
      }
    else
      fputs (req->remote_ip_addr, fp);
}

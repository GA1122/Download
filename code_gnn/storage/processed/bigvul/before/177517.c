 static int parse_dim(char *v, int *width, int *height) {
   char *x = strchr(v, 'x');
   if (x == NULL)
    x = strchr(v, 'X');
 if (x == NULL)
 return 0;
 *width = atoi(v);
 *height = atoi(&x[1]);
 if (*width <= 0 || *height <= 0)
 return 0;
 else
 return 1;
}

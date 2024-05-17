 static bool valid_user_field(const char *p, size_t l) {
         const char *a;
         

         
        if (l <= 0)
                return false;

         
        if (l > 64)
                return false;

         
        if (p[0] == '_')
                return false;

         
        if (p[0] >= '0' && p[0] <= '9')
                return false;

         
        for (a = p; a < p + l; a++)
                if (!((*a >= 'A' && *a <= 'Z') ||
                      (*a >= '0' && *a <= '9') ||
                      *a == '_'))
                        return false;

        return true;
}

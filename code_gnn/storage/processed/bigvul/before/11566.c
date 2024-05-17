bool path_equal(const char *a, const char *b) {
        assert(a);
        assert(b);

        if ((a[0] == '/') != (b[0] == '/'))
                return false;

        for (;;) {
                size_t j, k;

                a += strspn(a, "/");
                b += strspn(b, "/");

                if (*a == 0 && *b == 0)
                        return true;

                if (*a == 0 || *b == 0)
                        return false;

                j = strcspn(a, "/");
                k = strcspn(b, "/");

                if (j != k)
                        return false;

                if (memcmp(a, b, j) != 0)
                        return false;

                a += j;
                b += k;
        }
}

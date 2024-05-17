bool fstype_is_network(const char *fstype) {
        static const char * const table[] = {
                "cifs",
                "smbfs",
                "ncpfs",
                "nfs",
                "nfs4",
                "gfs",
                "gfs2"
        };

        unsigned i;

        for (i = 0; i < ELEMENTSOF(table); i++)
                if (streq(table[i], fstype))
                        return true;

        return false;
}

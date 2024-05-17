        static bool equal(const G* a, const G* b)
        {
            return unsafeHandleFromRawValue(a) == unsafeHandleFromRawValue(b);
        }

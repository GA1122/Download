        static unsigned hash(const G* key)
        {
            return static_cast<unsigned>(unsafeHandleFromRawValue(key)->GetIdentityHash());
        }

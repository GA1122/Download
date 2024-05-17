 static ThreadSpecific<BlobURLOriginMap>& originMap()
 {
    AtomicallyInitializedStatic(BlobOriginCache*, cache = new BlobOriginCache);

    AtomicallyInitializedStatic(ThreadSpecific<BlobURLOriginMap>*, map = new ThreadSpecific<BlobURLOriginMap>);
    return *map;
}

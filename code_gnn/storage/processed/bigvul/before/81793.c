void streamFreeConsumer(streamConsumer *sc) {
    raxFree(sc->pel);  
    sdsfree(sc->name);
    zfree(sc);
}

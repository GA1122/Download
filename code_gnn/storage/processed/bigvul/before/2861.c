ProcShmAttachFd(ClientPtr client)
{
    int fd;
    ShmDescPtr shmdesc;
    REQUEST(xShmAttachFdReq);
    struct stat statb;

    SetReqFds(client, 1);
    REQUEST_SIZE_MATCH(xShmAttachFdReq);
    LEGAL_NEW_RESOURCE(stuff->shmseg, client);
    if ((stuff->readOnly != xTrue) && (stuff->readOnly != xFalse)) {
        client->errorValue = stuff->readOnly;
        return BadValue;
    }
    fd = ReadFdFromClient(client);
    if (fd < 0)
        return BadMatch;

    if (fstat(fd, &statb) < 0 || statb.st_size == 0) {
        close(fd);
        return BadMatch;
    }

    shmdesc = malloc(sizeof(ShmDescRec));
    if (!shmdesc) {
        close(fd);
        return BadAlloc;
    }
    shmdesc->is_fd = TRUE;
    shmdesc->addr = mmap(NULL, statb.st_size,
                         stuff->readOnly ? PROT_READ : PROT_READ|PROT_WRITE,
                         MAP_SHARED,
                         fd, 0);

    close(fd);
    if (shmdesc->addr == ((char *) -1)) {
        free(shmdesc);
        return BadAccess;
    }

    shmdesc->refcnt = 1;
    shmdesc->writable = !stuff->readOnly;
    shmdesc->size = statb.st_size;
    shmdesc->resource = stuff->shmseg;

    shmdesc->busfault = busfault_register_mmap(shmdesc->addr, shmdesc->size, ShmBusfaultNotify, shmdesc);
    if (!shmdesc->busfault) {
        munmap(shmdesc->addr, shmdesc->size);
        free(shmdesc);
        return BadAlloc;
    }

    shmdesc->next = Shmsegs;
    Shmsegs = shmdesc;

    if (!AddResource(stuff->shmseg, ShmSegType, (void *) shmdesc))
        return BadAlloc;
    return Success;
}

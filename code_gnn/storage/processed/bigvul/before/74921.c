acl_new_entry(struct archive_acl *acl,
    int type, int permset, int tag, int id)
{
	struct archive_acl_entry *ap, *aq;

	 
	if (type & ARCHIVE_ENTRY_ACL_TYPE_NFS4) {
		if (acl->acl_types & ~ARCHIVE_ENTRY_ACL_TYPE_NFS4) {
			return (NULL);
		}
		if (permset &
		    ~(ARCHIVE_ENTRY_ACL_PERMS_NFS4
			| ARCHIVE_ENTRY_ACL_INHERITANCE_NFS4)) {
			return (NULL);
		}
	} else	if (type & ARCHIVE_ENTRY_ACL_TYPE_POSIX1E) {
		if (acl->acl_types & ~ARCHIVE_ENTRY_ACL_TYPE_POSIX1E) {
			return (NULL);
		}
		if (permset & ~ARCHIVE_ENTRY_ACL_PERMS_POSIX1E) {
			return (NULL);
		}
	} else {
		return (NULL);
	}

	 
	switch (tag) {
	case ARCHIVE_ENTRY_ACL_USER:
	case ARCHIVE_ENTRY_ACL_USER_OBJ:
	case ARCHIVE_ENTRY_ACL_GROUP:
	case ARCHIVE_ENTRY_ACL_GROUP_OBJ:
		 
		break;
	case ARCHIVE_ENTRY_ACL_MASK:
	case ARCHIVE_ENTRY_ACL_OTHER:
		 
		if (type & ~ARCHIVE_ENTRY_ACL_TYPE_POSIX1E) {
			return (NULL);
		}
		break;
	case ARCHIVE_ENTRY_ACL_EVERYONE:
		 
		if (type & ~ARCHIVE_ENTRY_ACL_TYPE_NFS4) {
			return (NULL);
		}
		break;
	default:
		 
		return (NULL);
	}

	if (acl->acl_text_w != NULL) {
		free(acl->acl_text_w);
		acl->acl_text_w = NULL;
	}
	if (acl->acl_text != NULL) {
		free(acl->acl_text);
		acl->acl_text = NULL;
	}

	 
	ap = acl->acl_head;
	aq = NULL;
	while (ap != NULL) {
		if (((type & ARCHIVE_ENTRY_ACL_TYPE_NFS4) == 0) &&
		    ap->type == type && ap->tag == tag && ap->id == id) {
			if (id != -1 || (tag != ARCHIVE_ENTRY_ACL_USER &&
			    tag != ARCHIVE_ENTRY_ACL_GROUP)) {
				ap->permset = permset;
				return (ap);
			}
		}
		aq = ap;
		ap = ap->next;
	}

	 
	ap = (struct archive_acl_entry *)calloc(1, sizeof(*ap));
	if (ap == NULL)
		return (NULL);
	if (aq == NULL)
		acl->acl_head = ap;
	else
		aq->next = ap;
	ap->type = type;
	ap->tag = tag;
	ap->id = id;
	ap->permset = permset;
	acl->acl_types |= type;
	return (ap);
}

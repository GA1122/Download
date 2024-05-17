NTSTATUS TCCreateDeviceObject (PDRIVER_OBJECT DriverObject,
		       PDEVICE_OBJECT * ppDeviceObject,
		       MOUNT_STRUCT * mount)
{
	UNICODE_STRING ntUnicodeString;
	WCHAR ntname[32];
	PEXTENSION Extension;
	NTSTATUS ntStatus;
	ULONG devChars = 0;
#if defined (DEBUG) || defined (DEBUG_TRACE)
	WCHAR dosname[32];
#endif

	Dump ("TCCreateDeviceObject BEGIN\n");
	ASSERT (KeGetCurrentIrql() == PASSIVE_LEVEL);

	TCGetNTNameFromNumber (ntname, sizeof(ntname),mount->nDosDriveNo);
	RtlInitUnicodeString (&ntUnicodeString, ntname);
#if defined (DEBUG) || defined (DEBUG_TRACE)
	TCGetDosNameFromNumber (dosname, sizeof(dosname),mount->nDosDriveNo, DeviceNamespaceDefault);
#endif

	devChars = FILE_DEVICE_SECURE_OPEN;
	devChars |= mount->bMountReadOnly ? FILE_READ_ONLY_DEVICE : 0;
	devChars |= mount->bMountRemovable ? FILE_REMOVABLE_MEDIA : 0;

#if defined (DEBUG) || defined (DEBUG_TRACE)
	Dump ("Creating device nt=%ls dos=%ls\n", ntname, dosname);
#endif

	ntStatus = IoCreateDevice (
					  DriverObject,			 
					  sizeof (EXTENSION),	 
					  &ntUnicodeString,		 
					  FILE_DEVICE_DISK,		 
					  devChars,				 
					  FALSE,				 
					  ppDeviceObject);		 

	if (!NT_SUCCESS (ntStatus))
	{
		Dump ("TCCreateDeviceObject NTSTATUS = 0x%08x END\n", ntStatus);
		return ntStatus; 
	}
	 

	(*ppDeviceObject)->Flags |= DO_DIRECT_IO;
	(*ppDeviceObject)->StackSize += 6;		 

	 
	Extension = (PEXTENSION) (*ppDeviceObject)->DeviceExtension;
	memset (Extension, 0, sizeof (EXTENSION));

	Extension->IsVolumeDevice = TRUE;
	Extension->nDosDriveNo = mount->nDosDriveNo;
	Extension->bRemovable = mount->bMountRemovable;
	Extension->PartitionInInactiveSysEncScope = mount->bPartitionInInactiveSysEncScope;
	Extension->SystemFavorite = mount->SystemFavorite;

	KeInitializeEvent (&Extension->keCreateEvent, SynchronizationEvent, FALSE);
	KeInitializeSemaphore (&Extension->RequestSemaphore, 0L, MAXLONG);
	KeInitializeSpinLock (&Extension->ListSpinLock);
	InitializeListHead (&Extension->ListEntry);
	IoInitializeRemoveLock (&Extension->Queue.RemoveLock, 'LRCV', 0, 0);

	VirtualVolumeDeviceObjects[mount->nDosDriveNo] = *ppDeviceObject;

	Dump ("TCCreateDeviceObject STATUS_SUCCESS END\n");

	return STATUS_SUCCESS;
}

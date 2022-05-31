#include <ntddk.h>
#include "Header.h"

void
unload(
	_In_ PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
	IoDeleteDevice(deviceobj);
	IoDeleteSymbolicLink(&symlink);
	KdPrint(("driver Unloaded\n"));
}

_Use_decl_annotations_
NTSTATUS
IomCreateClose(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp) // deal with i/o manager irps
{
	UNREFERENCED_PARAMETER(DeviceObject);
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	switch (stack->Parameters.DeviceIoControl.IoControlCode)
	{
		case IRP_MJ_CREATE:
		{
			DbgPrint("IRP_MJ_CREATE");
			break;
		}
		case IRP_MJ_CLOSE:
		{
			DbgPrint("IRP_MJ_CLOSE");
			break;
		}
	}
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return (Irp->IoStatus.Status);
}

_Use_decl_annotations_
NTSTATUS
DeviceChangeControl(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Irp);

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	DbgPrint("CIA is here \n");
	switch (stack->Parameters.DeviceIoControl.IoControlCode)
	{
	case IOCTL_CIA_CONTROL:
	{
		DbgPrint("CIA \n");
		break;
	}
	default:
		break;
	}
	return STATUS_SUCCESS;
}

DriverEntry
(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);
	DriverObject->DriverUnload = unload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = IomCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = IomCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceChangeControl;

	DbgPrint("Driver loaded\n");

	if (!NT_SUCCESS(IoCreateDevice(DriverObject, 0, &devname, 0x00000022, FILE_DEVICE_SECURE_OPEN, FALSE, &deviceobj)))
	{
		DbgPrint("Error while creating Device\n");
		return (STATUS_DEVICE_HARDWARE_ERROR);
	}
	if (!NT_SUCCESS(IoCreateSymbolicLink(&symlink, &devname)))
	{
		DbgPrint("Error while creating Symboliclink\n");
		IoDeleteDevice(deviceobj);
		return (STATUS_DEVICE_HARDWARE_ERROR);
	}
	return STATUS_SUCCESS;
}

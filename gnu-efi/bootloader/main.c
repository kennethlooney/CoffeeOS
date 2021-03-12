#include <efi.h>
#include <efilib.h>
#include <elf.h>


typedef unsigned long long size_t;
typedef struct
{
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;

} FRAMEBUFFER;
EFI_HANDLE ImageHandle;
EFI_SYSTEM_TABLE* SystemTable;

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

#define PSF2_MAGIC0 0x72
#define PSF2_MAGIC1 0xb5
#define PSF2_MAGIC2 0x4a
#define PSF2_MAGIC3 0x86
#define PSF2_HAS_UNICODE_TABLE 0x01
#define PSF2_MAXVERSION 0
#define PSF2_SEPERATOR 0xFF
#define PSF2_STARTSEQ 0xFE

 
typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF1_HEADER;
typedef struct {
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;
 

 
FRAMEBUFFER framebuffer;
FRAMEBUFFER* InitializeGOP()
{
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status;
	status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
	if(EFI_ERROR(status))
	{
		Print(L"Unable to locate graphics output protocol.\r\n");
		return NULL;
	} else {
		Print(L"Graphics output protocol located.\r\n");
	}

	framebuffer.BaseAddress = (void*)gop->Mode->FrameBufferBase;
	framebuffer.BufferSize = gop->Mode->FrameBufferSize;
	framebuffer.Width = gop->Mode->Info->HorizontalResolution;
	framebuffer.Height = gop->Mode->Info->VerticalResolution;
	framebuffer.PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

	return &framebuffer;
}
/**
 * @brief Tries to load the specified file on the volume. In UEFI directories
 * are treated as files.
 * 
 * @param Directory 
 * @param Path 
 * @return EFI_FILE* 
 */
EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path)
{
	EFI_FILE* LoadedFile;
	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem);

	// If the directory is NULL then we are setting the directory to
	// the root of the file system.
	if(Directory == NULL)
	{
		FileSystem->OpenVolume(FileSystem, &Directory);
	}
	
	// Open the directory and loading the file into LoadedFile
	EFI_STATUS s = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);

	if(s != EFI_SUCCESS) {
		// We could not load the specified file so return NULL.
		return NULL;
	} 

	// We did find the file so return it.
	return LoadedFile;
}
PSF1_FONT* LoadPsf1Font(EFI_FILE* Directory, CHAR16* Path)
{
	EFI_FILE* font = LoadFile(Directory, Path);
	if(font == NULL) return NULL;

	PSF1_HEADER* fontHeader;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_HEADER), (void**)&fontHeader);
	UINTN size = sizeof(PSF1_HEADER);
	font->Read(font, &size, fontHeader);

	if(fontHeader->magic[0] != PSF1_MAGIC0 || fontHeader->magic[1] != PSF1_MAGIC1) {
		return NULL;
	}
	UINTN glyphBufferSize = fontHeader->charsize * 256;
	if(fontHeader->mode == 1)
	{
		glyphBufferSize = fontHeader->charsize * 512;
	}
	void* glyphBuffer;
	{
		font->SetPosition(font, sizeof(PSF1_HEADER));
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
		font->Read(font, &glyphBufferSize, glyphBuffer);
	}

	PSF1_FONT* finishedFont;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&finishedFont);
	finishedFont->psf1_Header = fontHeader;
	finishedFont->glyphBuffer = glyphBuffer;
	return finishedFont;
}

 

int memcmp(const void* aptr, const void* bptr, size_t n)
{
	const unsigned char* a = aptr, *b = bptr;
	for(size_t i = 0; i < n; i++)
	{
		if(a[i] < b[i]) return -1;
		else if(a[i] > b[i]) return 1;
	}
	return 0;
}
/**
 * @brief The main entry point into the UEFI boot loader.
 * 
 * @param imageHandle 
 * @param systemTable 
 * @return EFI_STATUS 
 */
EFI_STATUS efi_main (EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
	 ImageHandle = imageHandle;
	 SystemTable = systemTable;
	 InitializeLib(ImageHandle, SystemTable);
	Print(L"Coffee Operating System 1.0\r\n");
	EFI_FILE* Kernel = LoadFile(NULL,L"kernel.elf");
	 if(Kernel == NULL)
	{
		Print(L"Could not read file.\r\n");
		return EFI_LOAD_ERROR;
	}
	Elf64_Ehdr header;
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);

		UINTN size = sizeof(header);
		Kernel->Read(Kernel, &size, &header);
	}
	if(
		memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT
	)
	{
		Print(L"Kernel format is bad\r\n");
		return EFI_LOAD_ERROR;
	}
	else {
		Print(L"Kernel header successfully verified\r\n");
	}
	Elf64_Phdr* phdrs;
	{
		Kernel->SetPosition(Kernel, header.e_phoff);
		UINTN size = header.e_phnum * header.e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		Kernel->Read(Kernel, &size, phdrs);

	}

	for(
		Elf64_Phdr* phdr = phdrs;
		(char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
		phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)
	)
	{
		switch(phdr->p_type)
		{
			case PT_LOAD:
			{
				int pages = (phdr->p_memsz + 0x1000 -1) / 0x1000;
				Elf64_Addr segment = phdr->p_paddr;
				SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);
				Kernel->SetPosition(Kernel, phdr->p_offset);
				UINTN size = phdr->p_filesz;
				Kernel->Read(Kernel, &size, (void*)segment);

			}break;
		}
	}
	FRAMEBUFFER* newFrameBuffer = InitializeGOP();
	Print(L"Kernel Loaded.\r\n");
	void (*KernelStart)(FRAMEBUFFER* buffer, PSF1_FONT* font) = ((__attribute__((sysv_abi)) void (*)(FRAMEBUFFER* buffer, PSF1_FONT* font) ) header.e_entry);
	PSF1_FONT* newFont = LoadPsf1Font(NULL,L"fonts\\zap-light18.psf");
	if(newFont == NULL)
	{
		Print(L"Font is not valid or is not found.\r\n");
	}
	else {
		Print(L"Font found. char size = %d\r\n", newFont->psf1_Header->charsize);
	}
	 
	

	KernelStart(newFrameBuffer, newFont);
	return EFI_SUCCESS;
}

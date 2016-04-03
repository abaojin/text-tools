
#import "C:\Program Files\Common Files\Microsoft Shared\OFFICE12\MSO.DLL" rename("RGB", "MSRGB")

#import "C:\Program Files\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB" raw_interfaces_only, rename("Reference", "ignorethis")//, rename("VBE", "testVBE")

#import "C:\Program Files\Microsoft Office\OFFICE12\EXCEL.EXE" rename_namespace("EXCEL") \
	rename("EOF", "EndOfFile") rename("RGB","ExcelRGB") rename("DialogBox","ExcelDialogBox") \
	rename("CopyFile","ExcelCopyFile") rename("ReplaceText","ExcelReplaceText")

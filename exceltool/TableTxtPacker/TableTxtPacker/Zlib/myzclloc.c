#include "zutil.h"
#include "malloc.h"

//extern void * GmCCalloc(int size, char* file, int line);
//extern void GmCFree(void* p);

void * GmCCalloc(int size, char* file, int line)
{
	/*
	char* buf = new char[size] ;
	memcpy( file , buf , size ) ;
	delete []buf ;
	return file ;
	*/

	void* bf = malloc( size ) ;
	return bf ;
}

void GmCFree(void* p)
{
	char* bf = (char*)p ;
	free( bf ) ;
}

voidpf zcalloc OF((voidpf opaque, unsigned items, unsigned size))
{
	return GmCCalloc(items * size, __FILE__ , __LINE__);
}
void   zcfree  OF((voidpf opaque, voidpf ptr))
{
	GmCFree(ptr);
}


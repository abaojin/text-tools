// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FTPROXY_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FTPROXY_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef FTPROXY_EXPORTS
#define FTPROXY_API __declspec(dllexport)
#else
#define FTPROXY_API __declspec(dllimport)
#endif

#include "ft2build.h"
#include "freetype/freetype.h"
#include <string>
#include <vector>

typedef void (*FTProxyLogFunc)( const char* str , int level ) ;

// This class is exported from the FTProxy.dll

struct FontConfigurationInfo
{
	int					index ;
	std::string			path ;
	FT_Face				face ;

	int					charGap ;
	int					lineGap ;

	int					szX , szY ;

	std::vector<std::string>	faceNames ;

	FontConfigurationInfo()
		:charGap(2)
		,lineGap(2)
		,szX(24)
		,szY(24)
	{
	}
};

#define CAL_RECT		1
#define AUTO_POS		2

struct UVInfo
{
	int					x , y ;
	int					w , h ;
	unsigned short		code ;
};

class FTPROXY_API CFTProxy
{
public:
	CFTProxy(void);
	~CFTProxy() ;

	bool Init( const char* param ) ;
	void Destroy() ;

	void SetCanvas( char* buf , int width , int height , int fmt ) ;
	void ClearCanvas() ;
	void SetFont( const char* name , int w , int h ) ;
	void SetFont( int cfg ) ;
	void SetSize( int w , int h ) ;
	int DrawGlyph( wchar_t code , int x , int y , int flag = 0 ) ;
	int DrawGlyphs( wchar_t* codes , int count , int x , int y ) ;

	int DrawAllGlyphs() ;

	int DrawGlyphsFromFile( const char* ) ;

	int GetCfgCount() ;

	const char* GetCfg( int ind ) ;

	void BeginRecordUV() ;
	int EndRecordUV() ;

	void SetLogFunc( FTProxyLogFunc func )
	{
		m_pLogFunc = func ;
	}

	void Log( const char* str , int level = 0 ) ;
protected:
	FT_Library				m_fontLib ;
	char*					m_pCanvasBuf ;
	int						m_nCanvasWidth ;
	int						m_nCanvasHeight ;
	int						m_nCanvasFmt ;

	FTProxyLogFunc			m_pLogFunc ;

	std::vector<FontConfigurationInfo*>		m_vecFontFaceCfg ;

	FontConfigurationInfo*	m_pCurCfg ;

	int						m_nAutoPosX ;			// 当外界没有指定绘制位置时，按内部计算结果自动增量
	int						m_nAutoPosY ;

	std::vector<UVInfo>		m_uv ;
	FILE					*m_pUVInfoFile ;
};

//

extern FTPROXY_API int nFTProxy;

extern "C" FTPROXY_API int fnFTProxy(void);

extern "C" FTPROXY_API _int32 FTProxy_CreateInstance( const char* prm ) ;
extern "C" FTPROXY_API void FTProxy_DestroyInstance( _int32 ) ;

extern "C" FTPROXY_API int FTProxy_SetCanvas( _int32 ptr , char buf[] , int w , int h , int fmt ) ;
extern "C" FTPROXY_API int FTProxy_ClearCanvas( _int32 ptr ) ;
extern "C" FTPROXY_API int FTProxy_SetFont( _int32 ptr , const char* fontFaceName , int w , int h ) ;
extern "C" FTPROXY_API int FTProxy_SetFontInd( _int32 ptr , int ind ) ;
extern "C" FTPROXY_API int FTProxy_SetSize( _int32 ptr , int w , int h ) ;
extern "C" FTPROXY_API int FTProxy_DrawGlyph( _int32 ptr , wchar_t code , int x , int y , int flag ) ;
extern "C" FTPROXY_API int FTProxy_DrawGlyphFromFile( _int32 ptr , const char* path ) ;
extern "C" FTPROXY_API int FTProxy_DrawGlyphs( _int32 ptr , wchar_t codes[] , int cnt , int x , int y ) ;
extern "C" FTPROXY_API int FTProxy_SetLogFunc( _int32 ptr , FTProxyLogFunc func ) ;

extern "C" FTPROXY_API int FTProxy_BeginRecordUV( _int32 ptr ) ;
extern "C" FTPROXY_API int FTProxy_EndRecordUV( _int32 ptr ) ;

extern "C" FTPROXY_API int FTProxy_GetCfgCnt( _int32 ptr ) ;
extern "C" FTPROXY_API const char* FTProxy_GetCfg( _int32 ptr , int ind ) ;


extern "C" FTPROXY_API int FTProxy_DrawAllGphphs( _int32 ptr ) ;

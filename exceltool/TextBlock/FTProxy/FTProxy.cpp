// FTProxy.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FTProxy.h"
#include "tinyxml.h"
#include <string>
#include <map>
#include "tchar.h"

// This is an example of an exported variable
FTPROXY_API int nFTProxy = 0 ;

// This is an example of an exported function.
FTPROXY_API int fnFTProxy(void)
{
	return 42 ;
}

// This is the constructor of a class that has been exported.
// see FTProxy.h for the class definition
CFTProxy::CFTProxy()
:m_fontLib(NULL)
,m_pCurCfg(NULL)
,m_pCanvasBuf(NULL)
,m_nCanvasWidth(0)
,m_nCanvasHeight(0)
,m_nCanvasFmt(0)
,m_nAutoPosX(0)
,m_nAutoPosY(0)
{
	return ;
}

CFTProxy::~CFTProxy()
{
	Destroy() ;
}

bool CFTProxy::Init( const char* param )
{
	int err = FT_Init_FreeType( &m_fontLib ) ;

	if( err )
	{
		Log( "FT_Init_FreeType() failed." , 9 ) ;
		return false ;
	}

	// load the configuration
	TiXmlDocument doc ;
	if( !doc.LoadFile( param ) )
		return false ;

	TiXmlElement *pRt = doc.FirstChildElement() ;

	TiXmlElement *pEle = pRt->FirstChildElement() ;

	while( pEle )
	{
		FontConfigurationInfo *cfg = new FontConfigurationInfo() ;
		m_vecFontFaceCfg.push_back( cfg ) ;

		//
		pEle->QueryIntAttribute( "index" , &cfg->index ) ;
		cfg->path = pEle->Attribute( "path" ) ;

		pEle->QueryIntAttribute( "width" , &cfg->szX ) ;
		pEle->QueryIntAttribute( "height" , &cfg->szY ) ;

		pEle->QueryIntAttribute( "charGap" , &cfg->charGap ) ;
		pEle->QueryIntAttribute( "lineGap" , &cfg->lineGap ) ;

		//
		FT_New_Face( m_fontLib , cfg->path.c_str() , 0 , &cfg->face ) ;


		printf( "The number of glyphs in face is : %d\n" , cfg->face->num_glyphs ) ;

		/*
		int error = FT_Set_Char_Size( cfg->face , // handle to face object 
										20*64,	// char_width in 1/64th of points 
										20*64, // char_height in 1/64th of points 
										0, // horizontal device resolution 
										0 ); // vertical device resolution  
		if( error != 0 )
		{
			printf( "FT_Set_Char_Size failed , error code is :%d\n" , error ) ;
		}
		*/

		FT_Set_Pixel_Sizes( cfg->face , cfg->szX , cfg->szY ) ;

		//
		pEle = pEle->NextSiblingElement() ;
	}

	if( m_vecFontFaceCfg.size() > 0 )
	{
		m_pCurCfg = m_vecFontFaceCfg[0] ;
	}

	return true ;
}

void CFTProxy::Destroy()
{
	int c = m_vecFontFaceCfg.size() ;

	for( int i = 0 ; i < c ; i ++ )
	{
		FontConfigurationInfo* pPtr = m_vecFontFaceCfg[i] ;
		FT_Done_Face( pPtr->face ) ;
		pPtr->face = NULL ;
	}

	m_vecFontFaceCfg.clear() ;
	m_pCurCfg = NULL ;

	FT_Done_FreeType( m_fontLib ) ;
	m_fontLib = NULL ;
}

void CFTProxy::Log( const char* str , int level )
{
	if( !m_pLogFunc )
		return ;

	m_pLogFunc( str , level ) ;
}

void CFTProxy::SetCanvas( char* buf , int width , int height , int fmt )
{
	m_pCanvasBuf = buf ;
	m_nCanvasWidth = width ;
	m_nCanvasHeight = height ;
	m_nCanvasFmt = fmt ;

	m_nAutoPosX = 0 ;
	m_nAutoPosY = 0 ;
}

void CFTProxy::ClearCanvas()
{
	if( m_pCanvasBuf )
		memset( m_pCanvasBuf , 0 , m_nCanvasHeight * m_nCanvasWidth ) ;
}

int CFTProxy::GetCfgCount()
{
	return m_vecFontFaceCfg.size() ;
}

const char* CFTProxy::GetCfg( int ind )
{
	int cnt = m_vecFontFaceCfg.size() ;
	if( ind < 0 || ind >= cnt )
		return "" ;

	FontConfigurationInfo* cfg = m_vecFontFaceCfg[ind] ;

	if( !cfg )
		return "Null Pointer." ;

	static char bf[1024] ={0};

	sprintf( bf , "TTF=%s,width=%d,height=%d,char gap=%d,line gap=%d" , cfg->path.c_str() , cfg->szX , cfg->szY , cfg->charGap , cfg->lineGap ) ;
	return bf ;
	//


	sprintf( bf , "<?xml version=\"1.0\" encoding=\"gbk\" standalone=\"yes\" ?>\
					  <Root>\
					  <Path value=\"%s\"/>\
					  <CharGap value=\"%d\"/>\
					  <LineGap value=\"%d\"/>\
					  <Width value=\"%d\"/>\
					  <Height value=\"%d\"/>\
					  </Root>" ,
					  cfg->path.c_str() ,
					  cfg->charGap , 
					  cfg->lineGap,
					  cfg->szX,
					  cfg->szY
					  ) ;

	return bf ;
}

void CFTProxy::BeginRecordUV()
{
	m_uv.clear() ;

	m_pUVInfoFile = fopen( "fontUV" , "wb+" ) ;
}

int CFTProxy::EndRecordUV()
{
	int ftCnt = m_uv.size() ;

	fwrite( &ftCnt , 4 , 1 , m_pUVInfoFile ) ;

	fwrite( &m_nCanvasWidth , 4 , 1 , m_pUVInfoFile ) ;
	fwrite( &m_nCanvasHeight , 4 , 1 , m_pUVInfoFile ) ;

	for( int i = 0 ; i < ftCnt ; i ++ )
	{
		UVInfo& uv = m_uv[i] ;

		fwrite( &(uv.code) , 2 , 1 , m_pUVInfoFile ) ;

		fwrite( &(uv.x) , 4 , 1 , m_pUVInfoFile ) ;
		fwrite( &(uv.y) , 4 , 1 , m_pUVInfoFile ) ;
	
		fwrite( &(uv.w) , 4 , 1 , m_pUVInfoFile ) ;
		fwrite( &(uv.h) , 4 , 1 , m_pUVInfoFile ) ;
	}

	fclose( m_pUVInfoFile ) ;

	return ftCnt ;
}

void ToLower( std::string & str ) ;
std::string AbstractFileName_2Lower( std::string inStr ) ;

void CFTProxy::SetFont( const char* name , int w , int h )
{
	if( w < 2 || h < 2 ||  w > 255 || h > 255 )
		return ;

	if( !name )
		return ;

	std::string ts = name ;
	ts = AbstractFileName_2Lower( ts ) ;

	//
	bool dealed = false ;

	int c = m_vecFontFaceCfg.size() ;

	for( int i = 0 ; i < c ; i ++ )
	{
		FontConfigurationInfo* pCfg = m_vecFontFaceCfg[i] ;
		std::string tts = AbstractFileName_2Lower( pCfg->path ) ;


		// 如果两个字体文件名字相同,尺寸也完全相同，视为同一个...
		// 如果是同名不同内容的文件，用户自己负责...
		if( tts == ts && w == pCfg->szX && h == pCfg->szY )
		{
			m_pCurCfg = pCfg ;
			dealed = true ;

			FT_Set_Pixel_Sizes( pCfg->face , w , h ) ;

			break ;
		}
	}

	// 如果没有找到并处理，那么新创建
	if( !dealed )
	{
		FT_Face fc ;
		int err = FT_New_Face( m_fontLib , name , 0 , &fc ) ;

		if( err != 0 )
		{
			::MessageBoxA( NULL , name , "SetFont failed." , MB_OK ) ;
			return ;
		}

		//
		FontConfigurationInfo* fci = new FontConfigurationInfo() ;
		fci->face = fc ;
		fci->index = 0 ;
		fci->path = name ;
		fci->szX = w ;
		fci->szY = h ;

		m_vecFontFaceCfg.push_back( fci ) ;

		m_pCurCfg = fci ;

		FT_Set_Pixel_Sizes( m_pCurCfg->face , w , h ) ;
	}
}

void CFTProxy::SetFont( int cfg )
{
	int cnt = m_vecFontFaceCfg.size() ;
	if( cfg < 0 || cfg >= cnt )
		return ;

	m_pCurCfg = m_vecFontFaceCfg[cfg] ;
	FT_Set_Pixel_Sizes( m_pCurCfg->face , m_pCurCfg->szX , m_pCurCfg->szY ) ;
	FT_Set_Char_Size( m_pCurCfg->face , m_pCurCfg->szX * 64 , m_pCurCfg->szY * 64 , 0 , 0 ) ;
}

void CFTProxy::SetSize( int w , int h )
{
	if( !m_pCurCfg )
		return ;

	FT_Set_Pixel_Sizes( m_pCurCfg->face , w , h ) ;
}

int CFTProxy::DrawGlyph( wchar_t code , int px , int py , int flag )
{
	if( !m_pCurCfg )
		return - 1 ;

	int glyph_index = FT_Get_Char_Index( m_pCurCfg->face , code ) ;
	
	if( glyph_index < 0 )
		return - 2 ;

	int err = FT_Load_Glyph( m_pCurCfg->face , glyph_index , FT_LOAD_DEFAULT ) ;

	if( err != 0 )
		return ( 1 << 16 ) | err ;

	FT_Render_Glyph( m_pCurCfg->face->glyph , FT_RENDER_MODE_NORMAL ) ;

	FT_Bitmap& bm = m_pCurCfg->face->glyph->bitmap ;

	//printf( "Bitmap's num_grays is : %d\n" , bm.num_grays ) ;
	printf( "Bitmap's pitch is : %d\n" , bm.pitch ) ;
	//printf( "Bitmap's pixel_mode is : %d\n" , bm.pixel_mode ) ;
	printf( "Bitmap's rows is : %d\n" , bm.rows ) ;
	printf( "Bitmap's width is : %d\n" , bm.width ) ;
	printf( "Face ascender is : %d\n" , m_pCurCfg->face->ascender >> 6 ) ;
	printf( "Face glyph->bitmap_left is : %d\n" , m_pCurCfg->face->glyph->bitmap_left ) ;
	printf( "Face glyph->bitmap_top is : %d\n" , m_pCurCfg->face->glyph->bitmap_top ) ;

	printf( "\n" ) ;

	if( !(flag & CAL_RECT) )
	{
		int osY = ( m_pCurCfg->face->ascender >> 6 ) - m_pCurCfg->face->glyph->bitmap_top ;

		// 如果自动排版
		if( flag & AUTO_POS )
		{
			// 判断换行
			if( ( m_nAutoPosX + m_pCurCfg->charGap + bm.width + m_pCurCfg->face->glyph->bitmap_left ) >= m_nCanvasWidth )
			{
				m_nAutoPosX = m_pCurCfg->charGap ;
				//m_nAutoPosY += ( m_pCurCfg->lineGap + ( m_pCurCfg->face->ascender >> 6 ) ) ;
				m_nAutoPosY += m_pCurCfg->szY ;
			}

			px = m_nAutoPosX ;
			py = m_nAutoPosY ;
		}

		for( int y = 0 ; y < bm.rows ; y ++ )
		{
			unsigned char* srchd = &bm.buffer[ y * bm.pitch ] ;

			int curY = y + osY + py ;

			if( curY < 0 )
				continue ;

			if( curY >= m_nCanvasHeight )
				break ;

			int curX = px + m_pCurCfg->face->glyph->bitmap_left ;

			int xReduce = 0 ;

			if( curX + bm.width >= m_nCanvasWidth )
			{
				xReduce = m_nCanvasWidth - ( curX + bm.width ) ;
			}

			char* dsthd = &m_pCanvasBuf[ curY * m_nCanvasWidth + curX ] ;

			memcpy( dsthd , srchd , bm.width - xReduce ) ;
		}

		// Out put uv info

		UVInfo uv ;
		uv.code = code ;
		uv.x = m_nAutoPosX ;
		uv.y = m_nAutoPosY ;
		uv.w = m_pCurCfg->face->glyph->bitmap_left + bm.width ;
		uv.h = m_pCurCfg->szY ;

		m_uv.push_back( uv ) ;

		//
		if( flag & AUTO_POS )
		{
			m_nAutoPosX += ( bm.width + m_pCurCfg->charGap ) ;
		}

		return 0 ;
	}

	int ret = ( bm.width << 16 ) | bm.rows ;

	return ret ;
}

int CFTProxy::DrawGlyphs( wchar_t* codes , int count , int x , int y )
{
	return 0 ;
}

int CFTProxy::DrawAllGlyphs()
{
	if( !m_pCurCfg )
		return - 1 ;

	int glyphCnt = m_pCurCfg->face->num_glyphs ;

	char cntBf[128] ;
	sprintf( cntBf , "%d" , glyphCnt ) ;

	::MessageBoxA( NULL , cntBf , "Total Glyphs' count." , MB_OK ) ;

	std::vector<std::pair<FT_ULong ,FT_UInt> >		infos ;

	{
		FT_UInt ind=0 ;
		FT_ULong code = FT_Get_First_Char( m_pCurCfg->face , &ind ) ;


		while( 1 )
		{
			code = FT_Get_Next_Char( m_pCurCfg->face , code , &ind ) ;

			infos.push_back( std::make_pair( code ,ind ) ) ;

			if( code == 0 || code < 0 )
				break ;
		}
	}

	int fontCountDrawn = 0 ;

	std::vector< std::pair< FT_ULong , FT_UInt > >::iterator itr = infos.begin() ;
	
	for( ; itr != infos.end() ; ++ itr ,fontCountDrawn ++ )
	{
		FT_ULong code = itr->first ;
		FT_UInt ind = itr->second ;

		int err = FT_Load_Glyph( m_pCurCfg->face , ind , FT_LOAD_DEFAULT ) ;

		if( err != 0 )
		{
			sprintf( cntBf , "%d" , ind ) ;
			::MessageBoxA( NULL , cntBf , "Load glyph failed." , MB_OK ) ;
			return ( 1 << 16 ) | err ;
		}

		FT_Render_Glyph( m_pCurCfg->face->glyph , FT_RENDER_MODE_NORMAL ) ;

		FT_Bitmap& bm = m_pCurCfg->face->glyph->bitmap ;

		// 判断换行
		if( ( m_nAutoPosX + bm.width + m_pCurCfg->face->glyph->bitmap_left ) >= m_nCanvasWidth )
		{
			m_nAutoPosX = 0 ;
			m_nAutoPosY += m_pCurCfg->szY + 2 ; //( m_pCurCfg->face->ascender >> 6 ) ;
		}

		int osY = ( m_pCurCfg->face->ascender >> 6 ) - m_pCurCfg->face->glyph->bitmap_top ;

		// 
		for( int y = 0 ; y < bm.rows ; y ++ )
		{
			unsigned char* srchd = &bm.buffer[ y * bm.pitch ] ;

			int curY = y + osY + m_nAutoPosY ;

			if( curY < 0 )
				continue ;

			if( curY >= m_nCanvasHeight )
				break ;

			int curX = m_nAutoPosX + m_pCurCfg->face->glyph->bitmap_left ;

			int xReduce = 0 ;

			if( curX + bm.width >= m_nCanvasWidth )
			{
				xReduce = m_nCanvasWidth - ( curX + bm.width ) ;
			}

			char* dsthd = &m_pCanvasBuf[ curY * m_nCanvasWidth + curX ] ;

			memcpy( dsthd , srchd , bm.width - xReduce ) ;
		}

		m_nAutoPosX += ( bm.width  ) ;

		if( m_nAutoPosY >= m_nCanvasHeight )
			break ;
	}

	//
	sprintf( cntBf , "Total font count is %d" , fontCountDrawn ) ;

	::MessageBoxA( NULL , cntBf , "Glyphs' count drawn ." , MB_OK ) ;

	FILE* pLogFile = fopen( "fontLog.log" , "w" ) ;
	
	if( pLogFile )
	{
		char buf[260] ;
		sprintf( buf , "Total glyphs' count is %d , %d of them drawn." , glyphCnt , fontCountDrawn ) ;
		fputs( "buf" , pLogFile ) ;
		fclose( pLogFile ) ;
	}

	return 0 ;
}

int CFTProxy::DrawGlyphsFromFile( const char* path )
{
	if( !m_pCurCfg )
		return - 1 ;

	//
	m_nAutoPosX = 0 ;
	m_nAutoPosY = 0 ;

	//
	//FILE* pF = fopen( path , "rb" ) ;
	FILE* pF;
	//if( !pF )
	//	return 0 ;

	std::vector<wchar_t>		uni_codes ;

	if( 1 ) 
	{
		wchar_t fnBf[512] ;
		memset( fnBf , 0 , 512 * sizeof(wchar_t) ) ;
		::MultiByteToWideChar( CP_ACP , 0 , "finalCode.txt" , - 1 , fnBf , 512 ) ;

		wchar_t flgBf[32] ;
		memset( flgBf , 0 , 32 * sizeof(wchar_t) ) ;
		::MultiByteToWideChar( CP_ACP , 0 , "r,ccs=UTF-8" , - 1 , flgBf , 32 ) ;

		FILE* pF1 = _wfopen( fnBf , flgBf ) ;
		//FILE* pF1 = fopen( "finalCode.txt" , "r,ccs=UTF-8" ) ;

		wchar_t wBf[1030] ;

		while( feof( pF1 ) == 0 )
		{
			memset( wBf , 0 , sizeof(wchar_t)*1030 ) ;

			fgetws( wBf , 1024 , pF1 ) ;

			int len = wcslen( wBf ) ;

			if( len == 0 )
				break ;

			for( int i = 0 ; i < len ; i ++ )
			{
				//wchar_t tmp = wBf[i*2] << 8 | wBf[i*2+1] ;

				wchar_t tmp = wBf[i] ;
				tmp = tmp >> 8 ;
				tmp |= ( wBf[i] << 8 ) ;

				uni_codes.push_back( tmp ) ;
			}
		}
	}

	wchar_t yi = _T('①') ;
	unsigned int yiInd = FT_Get_Char_Index( m_pCurCfg->face , yi ) ;

	if(0)
	{
		fseek( pF , 0 , SEEK_END ) ;
		unsigned int sz = ftell( pF ) ;

		fseek( pF , 0 , SEEK_SET ) ;

		int rdCnt = sz / 4096 ;
		int tailSz = 4096 ;

		if( sz % 4096 != 0 )
		{
			tailSz = sz % 4096 ;
			rdCnt ++ ;
		}

		unsigned short codeBuf[4096] ;
		wchar_t chars[4096] ;

		for( int i = 0 ; i < rdCnt ; i ++ )
		{
			// If this the last one .
			if( i == rdCnt - 1 )
			{
				fread( codeBuf , tailSz , 1 , pF ) ;
				memcpy( chars , codeBuf , tailSz ) ;

				for( int jj = 0 ; jj < tailSz/2 ; jj ++ )
					uni_codes.push_back( codeBuf[jj] ) ;
			}
			// Not the last one .
			else
			{
				fread( codeBuf , 4096 , 1 , pF ) ;

				for( int jj = 0 ; jj < 2048 ; jj ++ )
					uni_codes.push_back( codeBuf[jj] ) ;
			}
		}
	}

	//
	int cc = uni_codes.size() ;
	for( int i = 0 ; i < cc ; i ++ )
	{
		wchar_t ch = uni_codes[i] ;
		wchar_t cchh = ( ch >> 8 ) ;
		cchh |= ( ch << 8 ) ;
		uni_codes[i] = cchh ;
	}

	//
	std::vector<UVInfo>		vecUvs ;

	//
	char msgBf[128] ;

	int noFoundCnt = 0 ;

	//SetSize( 250 , 250 ) ;

	FILE* pLog = fopen( "fontRenderLog.log" , "w+" ) ;

	for( int i = 0 ; i < uni_codes.size() ; i ++ )
	{
		wchar_t chCode = uni_codes[i] ;

		//
		//wchar_t wbb[32] ={0};
		//::MultiByteToWideChar( CP_ACP , 0 , "啊" , -1 , wbb , 32 ) ;

		//

		//if( chCode == wbb[0] )
		//	continue ;

		//
		itoa( chCode , msgBf , 10 ) ;
		fputs( msgBf , pLog ) ;
		fputs( "\n" , pLog ) ;

		//

		unsigned int ind = FT_Get_Char_Index( m_pCurCfg->face , chCode ) ;
		if( ind == 0 )
			noFoundCnt ++ ;

		int err = FT_Load_Glyph( m_pCurCfg->face , ind , FT_LOAD_DEFAULT ) ;

		if( err != 0 )
		{
			sprintf( msgBf , "%d" , i ) ;
			::MessageBoxA( NULL , msgBf , "Load glyph failed." , MB_OK ) ;

			return ( 1 << 16 ) | err ;
		}

		FT_Render_Glyph( m_pCurCfg->face->glyph , FT_RENDER_MODE_NORMAL ) ;

		FT_Bitmap& bm = m_pCurCfg->face->glyph->bitmap ;

		// 判断换行
		if( ( m_nAutoPosX + bm.width + m_pCurCfg->face->glyph->bitmap_left ) >= m_nCanvasWidth )
		{
			m_nAutoPosX = 0 ;
			m_nAutoPosY += m_pCurCfg->szY + 2 ;
		}

		//
		int osY = ( m_pCurCfg->face->ascender >> 6 ) - m_pCurCfg->face->glyph->bitmap_top ;

		// 
		for( int y = 0 ; y < bm.rows ; y ++ )
		{
			unsigned char* srchd = &bm.buffer[ y * bm.pitch ] ;

			int curY = y + osY + m_nAutoPosY ;
			//int curY = y ;//+ osY + m_nAutoPosY ;

			if( curY < 0 )
				continue ;

			if( curY >= m_nCanvasHeight )
				break ;

			int curX = m_nAutoPosX + m_pCurCfg->face->glyph->bitmap_left ;

			int xReduce = 0 ;

			if( curX + bm.width >= m_nCanvasWidth )
			{
				xReduce = m_nCanvasWidth - ( curX + bm.width ) ;
			}

			char* dsthd = &m_pCanvasBuf[ curY * m_nCanvasWidth + curX ] ;

			memcpy( dsthd , srchd , bm.width - xReduce ) ;
		}

		// 保存 uv 信息
		UVInfo uv ;
		uv.code = chCode ;
		uv.x = m_nAutoPosX ;
		uv.y = m_nAutoPosY ;
		uv.w = bm.width + m_pCurCfg->face->glyph->bitmap_left ;
		uv.h = m_pCurCfg->szY;

		vecUvs.push_back( uv ) ;

		// 增量
		m_nAutoPosX += ( bm.width  ) ;

		if( m_nAutoPosX >= m_nCanvasWidth )
		{
			m_nAutoPosX = 0 ;
			m_nAutoPosY += m_pCurCfg->szY ;
		}

		if( m_nAutoPosY >= m_nCanvasHeight )
			break ;
	}

	fclose( pLog ) ;

	//
	sprintf( msgBf , "Total font count is %d" , uni_codes.size() ) ;
	::MessageBoxA( NULL , msgBf , "Glyphs' count drawn ." , MB_OK ) ;

	// Output UV information
	FILE* pUvInfo = fopen( "fontUV" , "wb+" ) ;
	int ftCnt = vecUvs.size() ;

	fwrite( &ftCnt , 4 , 1 , pUvInfo ) ;
	fwrite( &m_nCanvasWidth , 4 , 1 , pUvInfo ) ;
	fwrite( &m_nCanvasHeight , 4 , 1 , pUvInfo ) ;

	sprintf( msgBf , "Font count is %d , noFoundCnt is %d ." , ftCnt ,  noFoundCnt ) ;
	::MessageBoxA( NULL , msgBf , "uv count  ." , MB_OK ) ;

	std::map<int,int>		record ;

	for( int i = 0 ;i  < ftCnt ; i ++ )
	{
		UVInfo& uv = vecUvs[i] ;

		if( record.find(uv.code) != record.end() )
		{
			//::MessageBoxA( NULL , "unicode of character is conflicting..." , "" , MB_OK ) ;
		}

		fwrite( &(uv.code) , 2 , 1 , pUvInfo ) ;
		fwrite( &(uv.x) , 4 , 1 , pUvInfo ) ;
		fwrite( &(uv.y) , 4 , 1 , pUvInfo ) ;
		fwrite( &(uv.w) , 4 , 1 , pUvInfo ) ;
		fwrite( &(uv.h) , 4 , 1 , pUvInfo ) ;

		record[uv.code] = 1 ;

		//sprintf( msgBf , "code - %d , x - %d , y - %d , w - %d , h - %d" , uv.code , uv.x , uv.y , uv.w , uv.h ) ;
		//::MessageBoxA( NULL , msgBf , "uv count  ." , MB_OK ) ;
	}

	fclose( pUvInfo ) ;

	//
	FILE* pLogFile = fopen( "fontLog.log" , "w+" ) ;
	
	if( pLogFile )
	{
		char buf[260] ;
		sprintf( buf , "Total glyphs' count is %d ." , uni_codes.size() ) ;
		fputs( "buf\n" , pLogFile ) ;

		int cnt = vecUvs.size() ;
		for( int i = 0  ; i < cnt ; i ++ )
		{
			UVInfo& uv = vecUvs[i] ;
			sprintf( buf , "Code=%d , Pos= %d , %d  ,  Size= %d , %d : \n" , uv.code , uv.x , uv.y , uv.w , uv.h ) ;
			fputs( buf , pLogFile ) ;
		}

		fclose( pLogFile ) ;
	}

	return 0 ;
}

///////////////////////////

FTPROXY_API _int32 FTProxy_CreateInstance( const char* prm )
{
	CFTProxy* out = new CFTProxy() ;

	if( !out->Init( prm ) )
	{
		delete out ;
		return 0 ;
	}

	return (_int32)out ;
}

FTPROXY_API void FTProxy_DestroyInstance( _int32 ptr )
{
	if( ptr == 0 )
		return ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;
	delete pPtr ;
}

FTPROXY_API int FTProxy_SetCanvas( _int32 ptr , char buf[] , int w , int h , int fmt )
{
	if( ptr == 0 )
		return - 1 ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;
	pPtr->SetCanvas( buf , w , h , fmt ) ;

	return 0 ;
}

FTPROXY_API int FTProxy_ClearCanvas( _int32 ptr ) 
{
	if( ptr == 0 )
		return - 1 ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;
	pPtr->ClearCanvas() ;

	return 0 ;
}

FTPROXY_API int FTProxy_SetFont( _int32 ptr , const char* fontFaceName , int w , int h )
{
	if( ptr == 0 )
		return - 1 ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;
	pPtr->SetFont( fontFaceName , w, h ) ;

	return 0 ;
}

int FTProxy_SetFontInd( _int32 ptr , int ind )
{
	if( ptr == 0 )
		return - 1 ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;
	pPtr->SetFont( ind ) ;

	return 0 ;
}

FTPROXY_API int FTProxy_SetSize( _int32 ptr , int w , int h )
{
	if( ptr == 0 )
		return - 1 ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;
	pPtr->SetSize( w ,h ) ;

	return 0 ;
}

FTPROXY_API int FTProxy_DrawGlyph( _int32 ptr , wchar_t code , int x , int y , int flag )
{
	if( ptr == 0 )
		return - 1 ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;
	pPtr->DrawGlyph( code , x , y , flag ) ;

	return 0 ;
}

FTPROXY_API int FTProxy_DrawGlyphFromFile( _int32 ptr , const char* path )
{
	if( ptr == 0 )
		return - 1 ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;
	pPtr->DrawGlyphsFromFile( path ) ;

	return 0 ;
}

FTPROXY_API int FTProxy_DrawGlyphs( _int32 ptr , wchar_t codes[] , int cnt , int x , int y )
{
	if( ptr == 0 )
		return - 1 ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;
	pPtr->DrawGlyphs( codes , cnt , x , y ) ;

	return 0 ;
}

FTPROXY_API int FTProxy_SetLogFunc( _int32 ptr , FTProxyLogFunc func )
{
	if( ptr == 0 )
		return - 1 ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;
	pPtr->SetLogFunc( func ) ;

	return 0 ;
}

FTPROXY_API int FTProxy_BeginRecordUV( _int32 ptr )
{
	if( ptr == 0 )
		return - 1 ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;
	pPtr->BeginRecordUV() ;

	return 0 ;
}

FTPROXY_API int FTProxy_EndRecordUV( _int32 ptr )
{
	if( ptr == 0 )
		return - 1 ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;

	return pPtr->EndRecordUV() ;
}

FTPROXY_API int FTProxy_DrawAllGphphs( _int32 ptr )
{
	if( ptr == 0 )
		return - 1 ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;

	return pPtr->DrawAllGlyphs() ;
}


int FTProxy_GetCfgCnt( _int32 ptr )
{
	if( ptr == 0 )
		return - 1 ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;
	return pPtr->GetCfgCount() ;
}

const char* FTProxy_GetCfg( _int32 ptr , int ind )
{
	if( ptr == 0 )
		return "" ;

	CFTProxy* pPtr = (CFTProxy*)ptr ;
	return pPtr->GetCfg( ind ) ;
}

std::string AbstractFileName_2Lower( std::string inStr )
{
	std::string outStr ;

	int p0 = inStr.find('/') ;
	int p1 = inStr.find('\\') ;

	if( p0 < 0 && p1 < 0 )
	{

	}
	else
	{
		if( p1 > p0 )
			p0 = p1 ;

		outStr = &inStr.c_str()[ p0 + 1 ] ;
	}

	ToLower( outStr ) ;

	return outStr ;
}

void ToLower( std::string & str )
{
	int rel = 'a' - 'A' ;

	int c = str.length() ;

	char chs[2] ;
	chs[1] = 0 ;

	for( int i = 0 ; i < c ; i ++ )
	{
		if( str[i] >= 'A' && str[i] <= 'Z' )
		{
			char ch = str[i] + rel ;

			chs[0] = ch ;
			chs[1] = 0 ;

			str.replace( i , 1 , chs ) ;
		}
	}
}

/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2004 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.0 of the PHP license,       |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_0.txt.                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Stephan Schmidt <schst@php.net>                             |
  |          Carsten Lucke <luckec@php.net>                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_ID3_H
#define PHP_ID3_H

extern zend_module_entry id3_module_entry;
#define phpext_id3_ptr &id3_module_entry

#ifdef PHP_WIN32
#define PHP_ID3_API __declspec(dllexport)
#else
#define PHP_ID3_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(id3);
PHP_MSHUTDOWN_FUNCTION(id3);
PHP_MINFO_FUNCTION(id3);

PHP_FUNCTION(id3_get_tag);
PHP_FUNCTION(id3_get_version);
PHP_FUNCTION(id3_set_tag);
PHP_FUNCTION(id3_remove_tag);
PHP_FUNCTION(id3_get_genre_list);
PHP_FUNCTION(id3_get_genre_name);
PHP_FUNCTION(id3_get_genre_id);

/* macros */
#define BIT0(a)	(a & 1)
#define BIT1(a)	(a & 2)
#define BIT2(a)	(a & 4)
#define BIT3(a)	(a & 8)
#define BIT4(a)	(a & 16)
#define BIT5(a)	(a & 32)
#define BIT6(a)	(a & 64)
#define BIT7(a)	(a & 128)

/* function prototypes */
struct id3v2HdrFlags _php_id3v2_get_hdrFlags(php_stream *stream TSRMLS_DC);
int _php_id3v2_get_tagLength(php_stream *stream TSRMLS_DC);
int _php_bigEndian_to_Int(char* byteword, int bytewordlen, int synchsafe);
void _php_id3v1_get_tag(php_stream *stream , zval* return_value, int version TSRMLS_DC);
void _php_id3v2_get_tag(php_stream *stream , zval* return_value, int version TSRMLS_DC);
static int _php_id3_get_version(php_stream *stream TSRMLS_DC);
static int _php_id3_write_padded(php_stream *stream, zval **data, int length TSRMLS_DC);

/* constants */
const int ID3V2_BASEHEADER_LENGTH = 10;
const int ID3V2_IDENTIFIER_LENGTH = 3;
/* version constants */
const int ID3_V1_0	= 1;
const int ID3_V1_1	= 3;
const int ID3_V2_1	= 4;
const int ID3_V2_2	= 12;
const int ID3_V2_3	= 28;
const int ID3_V2_4	= 60;

/* id3v2x flags 
 * 
 * 1 = flag is set
 * 0 = flag isn't set
 * -1 = id3-version doesn't know about this flag
 */
struct id3v2HdrFlags {
	int	unsynch;
	int	extHdr;
	int experimental;
	int footer;
	int compression;
};

/* predefined genres */
const int ID3_GENRE_COUNT = 148;
char *id3_genres[148] = { "Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk", "Grunge", "Hip-Hop", "Jazz", "Metal", "New Age",
		"Oldies", "Other", "Pop", "R&B", "Rap", "Reggae", "Rock", "Techno", "Industrial", "Alternative", "Ska", "Death Metal", "Pranks", 
		"Soundtrack", "Euro-Techno", "Ambient", "Trip-Hop", "Vocal", "Jazz+Funk", "Fusion", "Trance", "Classical", "Instrumental", "Acid", 
		"House", "Game", "Sound Clip", "Gospel", "Noise", "Alternative Rock", "Bass", "Soul", "Punk", "Space", "Meditative", "Instrumental Pop", 
		"Instrumental Rock", "Ethnic", "Gothic", "Darkwave", "Techno-Industrial", "Electronic", "Pop-Folk", "Eurodance", "Dream", "Southern Rock", 
		"Comedy", "Cult", "Gangsta", "Top 40", "Christian Rap", "Pop/Funk", "Jungle", "Native US", "Cabaret", "New Wave", "Psychedelic", "Rave", 
		"Showtunes", "Trailer", "Lo-Fi", "Tribal", "Acid Punk", "Acid Jazz", "Polka", "Retro", "Musical", "Rock & Roll", "Hard Rock", "Folk", 
		"Folk-Rock", "National Folk", "Swing", "Fast Fusion", "Bebob", "Latin", "Revival", "Celtic", "Bluegrass", "Avantgarde", "Gothic Rock", 
		"Progressive Rock", "Psychedelic Rock", "Symphonic Rock", "Slow Rock", "Big Band", "Chorus", "Easy Listening", "Acoustic", "Humour", 
		"Speech", "Chanson", "Opera", "Chamber Music", "Sonata", "Symphony", "Booty Bass", "Primus", "Porn Groove", "Satire", "Slow Jam", "Club", 
		"Tango", "Samba", "Folklore", "Ballad", "Power Ballad", "Rhytmic Soul", "Freestyle", "Duet", "Punk Rock", "Drum Solo", "Acapella", 
		"Euro-House", "Dance Hall", "Goa", "Drum & Bass", "Club-House", "Hardcore", "Terror", "Indie", "BritPop", "Negerpunk", "Polsk Punk", 
		"Beat", "Christian Gangsta", "Heavy Metal", "Black Metal", "Crossover", "Contemporary C", "Christian Rock", "Merengue", "Salsa", 
		"Thrash Metal", "Anime", "JPop", "SynthPop" };
		
/* fseek positions */
const int ID3_SEEK_V1_TAG = -128;
const int ID3_SEEK_V1_TITLE = -125;
const int ID3_SEEK_V1_ARTIST = -95;
const int ID3_SEEK_V1_ALBUM = -65;
const int ID3_SEEK_V1_YEAR = -35;
const int ID3_SEEK_V1_COMMENT = -31;
const int ID3_SEEK_V1_TRACK = -2;
const int ID3_SEEK_V1_GENRE = -1;

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:	 

ZEND_BEGIN_MODULE_GLOBALS(id3)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(id3)
*/

/* In every utility function you add that needs to use variables 
   in php_id3_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as ID3_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define ID3_G(v) TSRMG(id3_globals_id, zend_id3_globals *, v)
#else
#define ID3_G(v) (id3_globals.v)
#endif

#endif	/* PHP_ID3_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

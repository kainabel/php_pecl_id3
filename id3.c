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
  |          Carsten Lucke <luckec@tool-garage.de>                       |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_id3.h"
#include "ext/standard/flock_compat.h"

/**
 * version constants
 */
const int ID3_V1_0 = 1;
const int ID3_V1_1 = 3;
const int ID3_V2   = 4;

/**
 * fseek positions
 */
const int ID3_SEEK_V1_TAG = -128;
const int ID3_SEEK_V1_TITLE = -125;
const int ID3_SEEK_V1_ARTIST = -95;
const int ID3_SEEK_V1_ALBUM = -65;
const int ID3_SEEK_V1_YEAR = -35;
const int ID3_SEEK_V1_COMMENT = -31;
const int ID3_SEEK_V1_TRACK = -2;
const int ID3_SEEK_V1_GENRE = -1;

char *id3_genres[148] = { "Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk", "Grunge", "Hip-Hop", "Jazz", "Metal", "New Age",
		"Oldies", "Other", "Pop", "R&B", "Rap", "Reggae", "Rock", "Techno", "Industrial", "Alternative", "Ska", "Death Metal", "Pranks", 
		"Soundtrack", "Euro-Techno", "Ambient", "Trip-Hop", "Vocal", "Jazz+Funk", "Fusion", "Trance", "Classical", "Instrumental", "Acid", 
		"House", "Game", "Sound Clip", "Gospel", "Noise", "Alternative Rock", "Bass", "Soul", "Punk", "Space", "Meditative", "Instrumental Pop", 
		"Instrumental Rock", "Ethnic", "Gothic", "Darkwave", "Techno-Industrial", "Electronic", "Pop-Folk", "Eurodance", "Dream", "Southern Rock", 
		"Comedy", "Cult", "Gangsta", "Top 40", "Christian Rap", "Pop/Funk", "Jungle", "Native US", "Cabaret", "New Wave", "Psychadelic", "Rave", 
		"Showtunes", "Trailer", "Lo-Fi", "Tribal", "Acid Punk", "Acid Jazz", "Polka", "Retro", "Musical", "Rock & Roll", "Hard Rock", "Folk", 
		"Folk-Rock", "National Folk", "Swing", "Fast Fusion", "Bebob", "Latin", "Revival", "Celtic", "Bluegrass", "Avantgarde", "Gothic Rock", 
		"Progressive Rock", "Psychedelic Rock", "Symphonic Rock", "Slow Rock", "Big Band", "Chorus", "Easy Listening", "Acoustic", "Humour", 
		"Speech", "Chanson", "Opera", "Chamber Music", "Sonata", "Symphony", "Booty Bass", "Primus", "Porn Groove", "Satire", "Slow Jam", "Club", 
		"Tango", "Samba", "Folklore", "Ballad", "Power Ballad", "Rhytmic Soul", "Freestyle", "Duet", "Punk Rock", "Drum Solo", "Acapella", 
		"Euro-House", "Dance Hall", "Goa", "Drum & Bass", "Club-House", "Hardcore", "Terror", "Indie", "BritPop", "Negerpunk", "Polsk Punk", 
		"Beat", "Christian Gangsta", "Heavy Metal", "Black Metal", "Crossover", "Contemporary C", "Christian Rock", "Merengue", "Salsa", 
		"Thrash Metal", "Anime", "JPop", "SynthPop" };

const int ID3_GENRE_COUNT = 148;

static int _php_id3_get_version(php_stream *stream TSRMLS_DC);
static int _php_id3_write_padded(php_stream *stream, zval **data, int length TSRMLS_DC);

/* If you declare any globals in php_id3.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(id3)
*/

/* True global resources - no need for thread safety here */
static int le_id3;

/* {{{ id3_functions[]
 *
 * Every user visible function must have an entry in id3_functions[].
 */
function_entry id3_functions[] = {
	PHP_FE(id3_get_version, NULL)
	PHP_FE(id3_get_tag,	NULL)
	PHP_FE(id3_set_tag, NULL)
	PHP_FE(id3_get_genre_list, NULL)
	PHP_FE(id3_get_genre_name, NULL)
	PHP_FE(id3_get_genre_id, NULL)
	PHP_FE(id3_remove_tag, NULL)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ id3_module_entry
 */
zend_module_entry id3_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"id3",
	id3_functions,
	PHP_MINIT(id3),
	PHP_MSHUTDOWN(id3),
	NULL,
	NULL,
	PHP_MINFO(id3),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ID3
ZEND_GET_MODULE(id3)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(id3)
{
	REGISTER_LONG_CONSTANT("ID3_V1_0", ID3_V1_0, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("ID3_V1_1", ID3_V1_1, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("ID3_V2", ID3_V2, CONST_CS|CONST_PERSISTENT);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(id3)
{
	return SUCCESS;
}

/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(id3)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "id3 support", "enabled");
	php_info_print_table_row(2, "Supported versions", "v1.0, v1.1");
	php_info_print_table_end();
}
/* }}} */

/* {{{ proto array id3_get_tag(string file)
   Returns an array containg all information from the id3 tag */
PHP_FUNCTION(id3_get_tag)
{
	zval *arg;
	php_stream *stream;
	int version = ID3_V1_0;
	int opened = 0;
	char tag[4];
	char title[31];
	char artist[31];
	char album[31];
	char year[5];
	char comment[31];
	unsigned char genre;
	char track;
	unsigned int bytes_read;
	char byte28, byte29;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|l", &arg, &version) == FAILURE) {
		return;
	}

	/**
	 * v2.0 will be implemented at later point
	 */
	if (version != ID3_V1_0 && version != ID3_V1_1) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_get_tag(): Unsupported version given");
		return;
	}


	switch(Z_TYPE_P(arg)) {
		case IS_STRING:
			stream = php_stream_open_wrapper(Z_STRVAL_P(arg), "rb", REPORT_ERRORS|ENFORCE_SAFE_MODE|STREAM_MUST_SEEK, NULL);
			opened = 1;
			break;
		case IS_RESOURCE:
			stream = php_stream_from_zval(stream, &arg)
			break;
		default:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_get_tag() expects parameter 1 to be string or resource");
			return;
	}

	/**
	 * invalid file
	 */
	if(!stream) {
		RETURN_FALSE;
	}

	php_stream_seek(stream, ID3_SEEK_V1_TAG, SEEK_END);
	php_stream_read(stream, tag, 3);

	if (strncmp(tag, "TAG", 3) != 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_get_tag() no ID3v1 tag found");
		if (opened == 1) {
			php_stream_close(stream);
		}
		return;
	}

	/**
	 * check for v1.1
	 */
	php_stream_seek(stream, -3, SEEK_END);
	php_stream_read(stream, &byte28, 1);
	php_stream_read(stream, &byte29, 1);
	if (byte28 == '\0' && byte29 != '\0') {
		version = ID3_V1_1;
	}

	array_init(return_value);
	
	/* title */
	php_stream_seek(stream, -125, SEEK_END);
	bytes_read = php_stream_read(stream, title, 30);
	if (strlen(title) < bytes_read) {
		bytes_read = strlen(title);
	}
	add_assoc_stringl(return_value, "title", title, bytes_read, 1);

	/* artist */
	bytes_read = php_stream_read(stream, artist, 30);
	if (strlen(artist) < bytes_read) {
		bytes_read = strlen(artist);
	}
	add_assoc_stringl(return_value, "artist", artist, bytes_read, 1);

	/* album */
	bytes_read = php_stream_read(stream, album, 30);
	if (strlen(album) < bytes_read) {
		bytes_read = strlen(album);
	}
	add_assoc_stringl(return_value, "album", album, bytes_read, 1);

	/* year */
	php_stream_read(stream, year, 4);
	if (strlen(year)>0) {
		add_assoc_stringl(return_value, "year", year, 4, 1);
	}
	
	/* comment */
	if (version == ID3_V1_1) {
		bytes_read = php_stream_read(stream, comment, 28);
	} else {
		bytes_read = php_stream_read(stream, comment, 30);
	}
	if (strlen(comment) < bytes_read) {
		bytes_read = strlen(comment);
	}
	add_assoc_stringl(return_value, "comment", comment, bytes_read, 1);
	
	/* track (only possible in v1.1) */
	if (version == ID3_V1_1) {
		php_stream_seek(stream, 1, SEEK_CUR);
		php_stream_read(stream, &track, 1);
		add_assoc_long(return_value, "track", (long)track);
	}

	/* genre */
	php_stream_read(stream, &genre, 1);
	add_assoc_long(return_value, "genre", (long)genre);

	if (opened == 1) {
		php_stream_close(stream);
	}
	return;
}
/* }}} */

/* {{{ proto boolean id3_set_tag(string file, array tag [, int version])
   Set an array containg all information from the id3 tag */
PHP_FUNCTION(id3_set_tag)
{
	zval *arg;
	zval *z_array;
	php_stream *stream;
	int version = ID3_V1_0;
	int old_version = 0;
	int opened = 0;

	HashTable *array;
	char *key;
	long index;
	zval **data;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "za|l", &arg, &z_array, &version) == FAILURE) {
		return;
	}

	/**
	 * v2.0 will be implemented at later point
	 */
	if (version != ID3_V1_0 && version != ID3_V1_1) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_get_tag(): Unsupported version given");
		return;
	}

	switch(Z_TYPE_P(arg)) {
		case IS_STRING:
			stream = php_stream_open_wrapper(Z_STRVAL_P(arg), "r+b", REPORT_ERRORS|ENFORCE_SAFE_MODE|STREAM_MUST_SEEK, NULL);
			opened = 1;
			break;
		case IS_RESOURCE:
			stream = php_stream_from_zval(stream, &arg)
			break;
		default:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_set_tag() expects parameter 1 to be string or resource");
			return;
	}

	/**
	 * invalid file
	 */
	if(!stream) {
		RETURN_FALSE;
	}

	/**
	 * No ID3v1.x tag found => append TAG and 125 zerobytes
	 * that will later store the supplied information
	 */
	old_version = _php_id3_get_version(stream TSRMLS_CC);
	if ((old_version & ID3_V1_0) == 0) {
		char blanks[125];
		php_stream_seek(stream, 0, SEEK_END);
		php_stream_write(stream, "TAG", 3);
        memset(blanks, 0, 125);
		php_stream_write(stream, blanks, 125);
	}
		
	array = HASH_OF(z_array);
	zend_hash_internal_pointer_reset(array);
	while (zend_hash_get_current_key(array, &key, &index, 0) == HASH_KEY_IS_STRING) {
		zend_hash_get_current_data(array, (void**) &data);
		
		if( strcmp("title", key) == 0) {
			convert_to_string(*data);
			if (strlen(Z_STRVAL_PP(data)) > 30) {
				php_error_docref(NULL TSRMLS_CC, E_NOTICE, "id3_set_tag(): title must be maximum of 30 characters, title gets truncated");
			}
			php_stream_seek(stream, ID3_SEEK_V1_TITLE, SEEK_END);
			php_stream_write(stream, Z_STRVAL_P(*data), 30);
		}

		if( strcmp("artist", key) == 0) {
			convert_to_string(*data);
			if (strlen(Z_STRVAL_PP(data)) > 30) {
				php_error_docref(NULL TSRMLS_CC, E_NOTICE, "id3_set_tag(): artist must be maximum of 30 characters, artist gets truncated");
			}
			php_stream_seek(stream, ID3_SEEK_V1_ARTIST, SEEK_END);
			_php_id3_write_padded(stream, data, 30 TSRMLS_CC);
		}

		if( strcmp("album", key) == 0) {
			convert_to_string(*data);
			if (strlen(Z_STRVAL_PP(data)) > 30) {
				php_error_docref(NULL TSRMLS_CC, E_NOTICE, "id3_set_tag(): album must be maximum of 30 characters, album gets truncated");
			}
			php_stream_seek(stream, ID3_SEEK_V1_ALBUM, SEEK_END);
		  	_php_id3_write_padded(stream, data, 30 TSRMLS_CC);
		}

		if( strcmp("comment", key) == 0) {
			long maxlen = 30;
			convert_to_string(*data);
			if (version == ID3_V1_1) {
				maxlen	=	28;
			}
			if (Z_STRLEN_PP(data) > maxlen) {
				php_error_docref(NULL TSRMLS_CC, E_NOTICE, "id3_set_tag(): comment must be maximum of 30 or 28 characters if v1.1 is used, comment gets truncated");
			}
			php_stream_seek(stream, ID3_SEEK_V1_COMMENT, SEEK_END);
			_php_id3_write_padded(stream, data, maxlen TSRMLS_CC);

		}

		if( strcmp("year", key) == 0) {
			convert_to_string(*data);
			if(strlen(Z_STRVAL_PP(data)) > 4) {
				php_error_docref(NULL TSRMLS_CC, E_NOTICE, "id3_set_tag(): year must be maximum of 4 characters, year gets truncated");
			}
			php_stream_seek(stream, ID3_SEEK_V1_YEAR, SEEK_END);
			_php_id3_write_padded(stream, data, 4 TSRMLS_CC);
		}

		if( strcmp("genre", key) == 0) {
			convert_to_long(*data);
			if (Z_LVAL_PP(data) > ID3_GENRE_COUNT) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_set_tag(): genre must not be greater than 147");
				zend_hash_move_forward(array);
				continue;
			}
			php_stream_seek(stream, ID3_SEEK_V1_GENRE, SEEK_END);
			php_stream_putc(stream, (char)(Z_LVAL_PP(data) & 0xFF));
		}

		if( strcmp("track", key) == 0) {
			convert_to_long(*data);
			if (version != ID3_V1_1) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_set_tag(): track may only be stored in ID3v1.1 tags");
				zend_hash_move_forward(array);
				continue;
			}
			if (Z_LVAL_PP(data) > 255) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_set_tag(): track must not be greater than 255");
				zend_hash_move_forward(array);
				continue;
			}
			php_stream_seek(stream, ID3_SEEK_V1_TRACK-1, SEEK_END);
			php_stream_putc(stream, '\0');
			php_stream_putc(stream, (char)(Z_LVAL_PP(data) & 0xFF));
		}

		zend_hash_move_forward(array);
	}
	if (opened == 1) {
		php_stream_close(stream);
	}
	
	RETURN_TRUE;
}
/* }}} */

/* {{{ write a zero-padded string to the stream */
int _php_id3_write_padded(php_stream *stream, zval **data, int length TSRMLS_DC)
{
	if (Z_STRLEN_PP(data)>length) {
		php_stream_write(stream, Z_STRVAL_PP(data), length);
	} else {	
		char blanks[30];
		memset(blanks, 0, 30);
		php_stream_write(stream, Z_STRVAL_PP(data), Z_STRLEN_PP(data));
		php_stream_write(stream, blanks, length - Z_STRLEN_PP(data));
	}
	return 1;	
}
/* }}} */

/* {{{ proto string id3_get_genre_name(int id)
 *	Returns genre name for an id */
PHP_FUNCTION(id3_get_genre_name)
{
	int id;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		return;
	}
	if (id >= ID3_GENRE_COUNT  || id < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_get_genre_name(): id must be between 0 and 147");
		return;
	}
	RETURN_STRING(id3_genres[(int)id], 1);
}
/* }}} */

/* {{{ proto int id3_get_genre_id(string name)
 *  *	Returns genre id for a genre name */
PHP_FUNCTION(id3_get_genre_id)
{
	char *name;
	int name_len;
	int i;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
		return;
	}
	for (i = 0; i < ID3_GENRE_COUNT; i++) {
		if (strcmp(name, id3_genres[i]) == 0) {
			RETURN_LONG(i);
		}
	}
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto array id3_get_genre_list()
 *  *	Returns an array with all possible genres */
PHP_FUNCTION(id3_get_genre_list)
{
	int i;
	array_init(return_value);
	for (i = 0; i < ID3_GENRE_COUNT; i++) {
		add_index_string(return_value, i, id3_genres[i], 1);
	}
	return;
}
/* }}} */

/* {{{ proto int id3_remove_tag()
 *  *	Returns true on success, otherwise false */
PHP_FUNCTION(id3_remove_tag)
{
	zval *arg;
	php_stream *stream;
	int opened = 0;
	int version = ID3_V1_0;
	int cutPos;
	int fd;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|l", &arg, &version) == FAILURE) {
		return;
	}

	/* v2.0 will be implemented at later point */
	if (version != ID3_V1_0 && version != ID3_V1_1) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_remove_tag(): Unsupported version given");
		return;
	}

	switch (Z_TYPE_P(arg)) {
		case IS_STRING:
			stream = php_stream_open_wrapper(Z_STRVAL_P(arg), "r+b", REPORT_ERRORS|ENFORCE_SAFE_MODE|STREAM_MUST_SEEK, NULL);
			opened = 1;
			break;
		case IS_RESOURCE:
			stream = php_stream_from_zval(stream, &arg)
			break;
		default:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_remove_tag() expects parameter 1 to be string or resource");
			return;
	}

	/* invalid file  */
	if (!stream) {
		RETURN_FALSE;
	}

	if ((_php_id3_get_version(stream TSRMLS_CC) & ID3_V1_0) == 0) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "id3_remove_tag() no ID3v1 tag found");
		if (opened == 1) {
			php_stream_close(stream);
		}
		RETURN_FALSE;
	}

	php_stream_seek(stream, ID3_SEEK_V1_TAG, SEEK_END);
	cutPos = php_stream_tell(stream);
	if(cutPos == -1) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_remove_tag() was unable to remove the existing id3-tag");
		if (opened == 1) {
			php_stream_close(stream);
		}
		return;
	}

	/* get the filedescriptor */
	/* XXX: AVOID DOING THIS!!! */
	if (FAILURE == php_stream_cast(stream, PHP_STREAM_AS_FD, (void **) &fd, REPORT_ERRORS)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_remove_tag() was unable to remove the existing id3-tag");
		if (opened == 1) {
			php_stream_close(stream);
		}
		return;
	}
	/* cut the stream right before TAG */
	if(ftruncate(fd, cutPos) == -1) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_remove_tag() was unable to remove the existing id3-tag");
		if (opened == 1) {
			php_stream_close(stream);
		}
		return;
	}

	if(opened == 1) {
		php_stream_close(stream);
	}
	RETURN_TRUE;
}
/* }}} */


/* {{{ proto int id3_get_version(string file)
   Returns version of the id3 tag */
PHP_FUNCTION(id3_get_version)
{
	zval *arg;
	php_stream *stream;
	int opened = 0;
	int version = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &arg) == FAILURE) {
		return;
	}

	switch(Z_TYPE_P(arg)) {
		case IS_STRING:
			stream = php_stream_open_wrapper(Z_STRVAL_P(arg), "rb", REPORT_ERRORS|ENFORCE_SAFE_MODE|STREAM_MUST_SEEK, NULL);
			opened = 1;
			break;
		case IS_RESOURCE:
			stream = php_stream_from_zval(stream, &arg)
			break;
		default:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "id3_get_version() expects parameter 1 to be string or resource");
			return;
	}

	/**
	 * invalid file
	 */
	if(!stream) {
		RETURN_FALSE;
	}

	version = _php_id3_get_version(stream TSRMLS_CC);
	
	if (opened == 1) {
		php_stream_close(stream);
	}
	RETURN_LONG(version);
}
/* }}} */


int _php_id3_get_version(php_stream *stream TSRMLS_DC)
{
	int version = 0;
	char buf[4];

	/**
	 * check for v1
	 */
	php_stream_seek(stream, ID3_SEEK_V1_TAG, SEEK_END);
	php_stream_read(stream, buf, 3);
	if (strncmp("TAG", buf, 3) == 0) {
		char byte28, byte29;
		version = version | ID3_V1_0;


		php_stream_seek(stream, -3, SEEK_END);
		php_stream_read(stream, &byte28, 1);
		php_stream_read(stream, &byte29, 1);
		if (byte28 == '\0' && byte29 != '\0') {
			version = version | ID3_V1_1;
		}
	}

	/**
	 * check for v2
	 */
	php_stream_seek(stream, 0, SEEK_SET);
	php_stream_read(stream, buf, 3);
	if (strncmp("ID3", buf, 3) == 0) {
		version = version | ID3_V2;
	}
	return version;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
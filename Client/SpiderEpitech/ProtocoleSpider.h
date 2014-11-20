#ifndef PROTOCOLE_H_
# define PROTOCOLE_H_

# define SERVER_PORT		4243
# define NB_MAGIC			42
# define MAJOR				2
# define MINOR				1
# define HEADER_SIZE		20
# define MAGIC_HEADER_SIZE	28

# include <cstdint>

typedef int32_t		t_client_id;
typedef uint32_t	t_token;
typedef uint32_t	t_size;
typedef uint32_t	t_msg_id;

enum	e_type
{
	LOG = 0,
	ID = 1,
	RESP_KO = 2,
	RESP_OK = 3,
	PAUSE = 4,
	RESTART = 5,
	MOVE = 6,
	FLUSH = 7,
	REMOVE = 8,
	PING = 9,
	PONG = 10
};

enum	e_state
{
	NONE = 0,
	PUSH = 1,
	RELEASE = 2
};

enum	e_log_input
{
	KEYSTROKE = 0,
	MOUSE = 1,
	FOCUS = 2
};

typedef struct	s_magic_header
{
	char			name[16];
	uint32_t		magic;
	uint32_t		major;
	uint32_t		minor;
}				t_magic_header;

typedef struct	s_header
{
	t_client_id	id_client;
	t_token		token;
	uint32_t		type;
	t_msg_id		id_msg;
	t_size		size;
}				t_header;

typedef struct	s_coord
{
	int32_t		x;
	int32_t		y;
}				t_coord;

typedef struct	s_log
{
	int64_t		time;
	uint32_t	input;
	uint32_t	state;
	char		value[64];
	t_coord		coord;
}				t_log;

#endif

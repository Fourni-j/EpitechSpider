#ifndef _SPIDER_HH_
# define _SPIDER_HH_

# include <cstdint>

typedef enum	e_type
  {
    LOG = 0,
    ID = 1,
    RESP_KO = 2,
    RESP_OK = 3,
    PAUSE = 4,
    RESTART = 5,
    MOVE = 6,
    FLUSH = 7,
    DELETE = 8,
    PING = 9,
    PONG = 10
  }		t_type;

typedef enum	e_state
  {
    NONE = 0,
    PUSH = 1,
    RELEASE = 2
  }		t_state;

typedef enum	e_log_input
  {
    KEYSTROKE = 0,
    MOUSE = 1,
    FOCUS = 2
  }		t_log_input;

struct	s_magic_header
{
  char		name[16];
  uint32_t	magicNb;
  uint32_t	version;
  uint32_t	subversion;
};

struct	s_header
{
  int32_t	id_client;
  uint32_t	token_id;
  uint32_t	type;
  uint32_t	msg_id;
  uint32_t	size;
};

struct	s_coord
{
  int32_t	x;
  int32_t	y;
};

struct	s_log
{
  int64_t		time;
  uint32_t		input;
  uint32_t		state;
  char			value[64];
  struct s_coord	coord;
};

#endif

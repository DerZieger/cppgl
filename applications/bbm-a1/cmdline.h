#pragma once

#include <string>
#include <unistd.h>

//! \brief Translated command line options
struct Cmdline
{
	bool verbose;	//!< wheather or not to be verbose
	std::string server_host, player_name;
	int res_x, res_y;

	Cmdline() : verbose(false), server_host("localhost"), res_x(1024), res_y(768) {
		char *login = getlogin();
		if (login == 0) player_name = "unknown";
		else            player_name = login;
	}
};

extern Cmdline cmdline;

int parse_cmdline(int argc, char **argv);


//
//  LocalLog.h
//  AsyncClientBoost
//
//  Created by Charles Fournier on 01/11/2014.
//  Copyright (c) 2014 Charles Fournier. All rights reserved.
//

#ifndef __AsyncClientBoost__Config__
#define __AsyncClientBoost__Config__

#include <boost\property_tree\json_parser.hpp>
#include <boost\property_tree\ptree.hpp>
#include <fstream>

class Config
{
public:
	Config();
	~Config();
	void	addIntegerToField(int data, std::string field);
	void	addIntegerToField(std::string data, std::string field);
	void	saveConfig();
	void	displayConfig();
	void	loadConfig();
	int		getId() const;
	int		getToken() const;
	std::string		getIp() const;

private:
	int _id;
	int _token;
	std::string _ip;
	boost::property_tree::ptree _pt;
};


#endif /* defined(__AsyncClientBoost__Config__) */

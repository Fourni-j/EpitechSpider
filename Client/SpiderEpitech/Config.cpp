#include "Config.h"

Config::Config()
{
	_id = -1;
	_token = 0;
	_ip = "10.16.252.229";
	_pt.put("ip", _ip);
}

Config::~Config()
{
}

void Config::addIntegerToField(int data, std::string field)
{
	_pt.put(field, data);
}

void Config::addIntegerToField(std::string data, std::string field)
{
	_pt.put(field, data);
}

void Config::displayConfig()
{
	std::ostringstream buf;
	boost::property_tree::write_json(buf, _pt, false);
	std::string json = buf.str();
	std::cout << "Config data :  " << json << std::endl;
}

void Config::saveConfig()
{
	std::ofstream ofstream("Config.spider");
	ofstream.flush();
	std::ostringstream buf;
	boost::property_tree::write_json(buf, _pt, false);
	std::string json = buf.str();
	ofstream << json;
	ofstream.close();
}

void Config::loadConfig()
{
	try {
		std::ifstream ifstream("Config.spider");
		std::string json;

		ifstream >> json;
		if (json.empty() == true)
			return;
		_pt.clear();
		std::istringstream is(json);
		boost::property_tree::read_json(is, _pt);

		_id = _pt.get<int>("id");
		_token = _pt.get<int>("token");
		_ip = _pt.get<std::string>("ip");
	}
	catch (std::exception &e)
	{
		std::cout << "Exception boost json : " << e.what() << std::endl;
		std::ofstream ofstream("Config.spider");
		ofstream.flush();
		ofstream.close();
	}
}

int	Config::getId() const
{
	return _id;
}

int Config::getToken() const
{
	return _token;
}

std::string Config::getIp() const
{
	return _ip;
}
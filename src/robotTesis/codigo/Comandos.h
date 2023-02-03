
#ifndef CODIGO_COMANDOS_H_
#define CODIGO_COMANDOS_H_
#include <stdio.h>
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <unistd.h>
#include "std_msgs/String.h"
#include "miRobot.h"
std::vector<std::string> split(const std::string &c,char d);
namespace gazebo {

	class Comandos {
		public:
			static bool procesar(std::string comando, MiRobot *);
			static std::string ruta;
	};

}

#endif 

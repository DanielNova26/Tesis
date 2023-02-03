
#include "Comandos.h"
#include <stdio.h>
#include "std_msgs/String.h"
#include <vector>
#include "miRobot.h"
namespace gazebo {
	std::string Comandos::ruta="";
	bool Comandos::procesar(std::string comando, MiRobot * robot){
		std::vector<std::string> partes=split(comando, ' ');
		if(partes.size()==0){
			gzerr<<"NO se ha indicado un comando válido."<< "\r\n";
	}else {
		switch(partes[0][0]){
			case 'd':
				if(partes.size()>1){
					robot->pintar(partes[1]);
				}
			break;
			case 'p':
			if (partes.size()>3){
			    robot->parametrizar(partes[1],partes[2], std::stod (partes[3]));
			}
			break;
			case 'm':
			if (partes.size()>2){
			    robot->mover(partes[1],std::stod (partes[2]));
			}
			break;
			case 'e':
			{
			if( ruta==""){
				ROS_INFO("Estas en %s",boost::filesystem::initial_path().string().c_str());
				boost::filesystem::path p=boost::filesystem::complete(partes[1]);
				ruta=p.parent_path().string();
				ROS_INFO("Ruta fija %s",ruta.c_str());
				ROS_INFO("Fichero: %s",partes[1].c_str());
			
			}else{
				partes[1]=ruta+"/"+partes[1];
			}
			    
			    std::ifstream stream (partes[1]);
			    if(stream.good ()){
			        std::string linea;
			        while(!stream.eof()){
			             linea.clear();
			             std::getline (stream, linea);
			            Comandos::procesar(linea, robot);
			        }
			      
			    }else{
					ROS_INFO("No se ha podido abrir el fichero %s",partes[1].c_str());

				}
				stream.close();
				}
			break;
			case 'w':			    			
				ROS_INFO("esperando");

			break;
			case 's':
					{
				std::string tiempo(partes[1]);
				ROS_INFO("durmiendo %d",std::stoi(tiempo));
				boost::this_thread::sleep(boost::posix_time::milliseconds(std::stoi(tiempo)));
				ROS_INFO("despierto");
						}

			break;
			}
		}
		return false;
	}
		
}


std::vector<std::string> split(const std::string &c,char d){
	std::vector<std::string> resultado;
	
	std::stringstream cs(c);
	std::string parte;
	while(std::getline(cs, parte,d)){
		resultado.push_back(parte);
	}
	
	return resultado;
}
#include "listner.h"
#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "ros/subscribe_options.h"
#include "std_msgs/String.h"

#include <stdio.h>
#include <gazebo/gazebo.hh>
#include "Comandos.h"
namespace gazebo{
	void Listner::init(MiRobot *robot){
		this->robot=robot;
		if(!ros::isInitialized()){
			int argc=0;
			char **argv=NULL;
			ros::init(argc,argv, "gazebo_client", ros::init_options::NoSigintHandler);
		}
		this->nodo.reset(new ros::NodeHandle("gazebo_client"));

		ros::SubscribeOptions so=ros::SubscribeOptions::create<std_msgs::String>(
				"/miRobot",
				1,
				boost::bind(&Listner::listner,this, _1),
				ros::VoidPtr(),
				&this->cola
		);
		this->subscriber=this->nodo->subscribe(so);

		ros::AdvertiseOptions ad=ros::AdvertiseOptions::create<std_msgs::String>(
						"/miRobot_m",
						1,
						&this->conexion,
						&this->desconexion,
						ros::VoidPtr(),
						&this->cola2
						);
		this->publishe=this->nodo->advertise(ad);

		this->threadColas=std::thread(std::bind(&Listner::thread, this));
	}
	void Listner::listner(const std_msgs::String::ConstPtr& msg){	
		std::string m=msg->data.c_str();
		Comandos::procesar(m, this->robot);
			   

	}
	void Listner::conexion(const ros::SingleSubscriberPublisher&){
		ROS_INFO("Me Conecto");
		
	}
	void Listner:: desconexion(const ros::SingleSubscriberPublisher&){
		ROS_INFO("Me Desconecto");
		
	}
	void Listner::thread(){
		static const double timeout=0.01;
		while(this->nodo->ok()){
			this->cola.callAvailable(ros::WallDuration(timeout));
			
				std_msgs::String m;
				std::stringstream ms;
		
				ms<<this->robot->getEstado()<<" ";
				m.data=ms.str();
				
				this->publishe.publish(m);
			
		}
	}
}

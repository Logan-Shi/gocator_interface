//ros dependencies
#include "gocator_3200_node.h"

//node main
int main(int argc, char **argv)
{
  //init ros
  ros::init(argc, argv, "gocator_3200_node");

  //create ros wrapper object
  Gocator3200Node gocator;

  //set node loop rate
  ros::Rate loop_rate(gocator.rate());

  //node loop 
  while ( ros::ok() )
  {
    //execute pending callbacks
    ros::spinOnce();
    //switch according run mode
    switch(gocator.runMode())
    {
      case SNAPSHOT:
        //check if pending snapshot request
        if ( gocator.isRequest() )
        {
          gocator.resetRequest();
          gocator.publish(); 
        }
        break;
  
      case PUBLISHER:
        gocator.publish();
        break;
  
      case SAVER:
        if (*(gocator.save_request) == 1)
        {
          gocator.saveShot();
        }
        break;
        
      default:
        break;
    }
  
    //publish camera field of view wire frame
    if (gocator.isFovViz())
    {
      gocator.publish_fov(); 
    }
  
    //relax to fit output rate
    loop_rate.sleep();
  }
  //exit program
  return 0;
}
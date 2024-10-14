#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <string.h>
#include <pcl/io/pcd_io.h>
// #include <pcl/point_types.h>
#include <pcl/io/io.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
// #include <pcl/filters/statistical_outlier_removal.h>   //统计滤波
// #include <pcl/filters/random_sample.h>    //随机采样
// #include <pcl/filters/voxel_grid.h>      //体素滤波
// #include <pcl/filters/passthrough.h>     //直通滤波
#include <pcl/visualization/pcl_visualizer.h>
#include <boost/thread/thread.hpp>
#include <pcl_conversions/pcl_conversions.h>
using namespace std;
boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer1(new pcl::visualization::PCLVisualizer("realtime pcl"));
 
ros::Publisher pub;
 
typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;
 
void cloud_cb (const sensor_msgs::PointCloud2ConstPtr& input)
{
   // 声明存储原始数据与滤波后的数据的点云的格式
  pcl::PCLPointCloud2* cloud = new pcl::PCLPointCloud2;    //原始的点云的数据格式
  pcl::PCLPointCloud2ConstPtr cloudPtr(cloud);
  // 转化为PCL中的点云的数据格式
  pcl_conversions::toPCL(*input, *cloud);
 
  pub.publish (*cloud);
 
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud1;
  cloud1.reset (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::fromROSMsg (*input, *cloud1);
//  pcl::visualization::CloudViewer viewer ("Simple Cloud Viewer");
//  viewer.showCloud(cloud1,"Simple Cloud Viewer");
  viewer1->removeAllPointClouds();  // 移除当前所有点云
  viewer1->addPointCloud(cloud1, "realtime pcl");
  viewer1->updatePointCloud(cloud1, "realtime pcl");
  viewer1->spinOnce(0.001);
 
}
 
int main (int argc, char** argv)
{
  // Initialize ROS
  ros::init (argc, argv, "pcl");
  ros::NodeHandle nh;
 
  // Create a ROS subscriber for the input point cloud
  //"/camera/depth/color/points"realsense发布的点云数据
  ros::Subscriber sub = nh.subscribe ("/rslidar_points", 10, cloud_cb);
  // Create a ROS publisher for the output point cloud
  pub = nh.advertise<pcl::PCLPointCloud2> ("output", 10);
  // Spin
  ros::spin ();
}

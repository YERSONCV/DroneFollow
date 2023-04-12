// .pf.cpp
#include <iostream>
#include <random>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "ParticleFilter/particle.h"
#include "utils/functions.h"
#include "VanishingPoint/vanishpoint.h"
#include <opencv2/imgproc/types_c.h>
using namespace std;
using namespace cv;


funciones f;
vanishpoint vp;

Mat videoFrame;
Mat Frame;
Mat capFrame;
Mat refFrame;
  Mat refMask;
Size objectSize(40,40);
Point roiPoint = Point2f(363,165);
Point MroiPoint;

int main()
{
  //  //638 × 358
  //  VideoCapture video("/home/yrsn/Videos/out.mp4");
  //  VideoCapture cap("/home/yrsn/Videos/video_.mp4");

  //  if(!video.isOpened())
  //    return -1;

  //  video >> videoFrame;
  //  cap >> frame;

  //  vp.vp(frame,mask);

  ////  cv::cvtColor(mask, frame, cv::COLOR_GRAY2BGR);

  //  imshow("OUTPUT",videoFrame);
  //  imshow("MASK",mask);
  //  //  // get center of image
  ////  targetWithrectangle = f.getCenter(frame,roiPoint);

  int nParticles = 3000;
  int mParticles = 2000;
  Mat Resampling;
  Mat Mesampling;


  VideoCapture video("/home/yrsn/Videos/out.mp4");
  VideoCapture cap("/home/yrsn/Videos/video_.mp4");

  video >> videoFrame;
  cap >> Frame;


  vp.vp(Frame,refMask);
  refFrame = imread("/home/yrsn/Videos/out.png", COLOR_BGR2RGB);

  imshow("refmask",refMask);
  cv::cvtColor(refMask, refMask, cv::COLOR_GRAY2BGR);

  f.getCenter(refMask,MroiPoint);


  Particle particleAlgo(refFrame, videoFrame, nParticles, objectSize, roiPoint);



  capFrame = refMask.clone();
  Particle MyparticleAlgo(refMask, capFrame, mParticles, objectSize, MroiPoint);


  int KeyEvent;


  while(!videoFrame.empty())
    {
      //imshow("videoFrame",videoFrame);
      Resampling = videoFrame.clone();
      Mesampling = capFrame.clone();


      imshow("videoFrame",videoFrame);
      imshow("capFrame",capFrame);

      //To initialize the particles
      if(video.get(CAP_PROP_POS_FRAMES)==1)
        {

          particleAlgo.setInputImage(videoFrame);
          particleAlgo.generateParticlesInit();
          Resampling = particleAlgo.getParticleImage().clone();
           namedWindow("Particles",WINDOW_NORMAL);
           imshow("Particles", Resampling);
          videoFrame = particleAlgo.getTarget().clone();




          MyparticleAlgo.setInputImage(capFrame);
          MyparticleAlgo.generateParticlesInit();
          Mesampling = MyparticleAlgo.getParticleImage().clone();
           namedWindow("MParticles",WINDOW_NORMAL);
           imshow("MParticles", Mesampling);
          capFrame = MyparticleAlgo.getTarget().clone();
        }

      //Frame by frame tracking
      if(video.get(CAP_PROP_POS_FRAMES)>1)
        {
          particleAlgo.setInputImage(videoFrame);
          particleAlgo.particleAlgo();
          videoFrame = particleAlgo.getTarget().clone();
          Resampling = particleAlgo.getParticleImage().clone();
          imshow("Particles", Resampling);



          MyparticleAlgo.setInputImage(capFrame);
          MyparticleAlgo.particleAlgo();
          videoFrame = MyparticleAlgo.getTarget().clone();
          Mesampling = MyparticleAlgo.getParticleImage().clone();
          imshow("MParticles", Mesampling);
        }

      namedWindow("Tracking",WINDOW_NORMAL);
      imshow("Tracking",videoFrame);

      KeyEvent = waitKey(33);
      if(KeyEvent == 27)
        {
          break;
        }
      else if(KeyEvent == 32)
        {
          waitKey(0);
        }

      video >> videoFrame;
      cap >> capFrame;
      vp.vp(capFrame,refMask);
      cv::cvtColor(refMask, capFrame, cv::COLOR_GRAY2BGR);
      //f.getCenter(refMask,MroiPoint);
      imshow("capFrameabajo",capFrame);
    }


  waitKey(0);
  return 0;

}

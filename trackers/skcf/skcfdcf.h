/**************************************************************************************************
 **************************************************************************************************
 
     GPL-3 License (https://www.tldrlegal.com/l/gpl-3.0)
     
     Copyright (c) 2015 Andrés Solís Montero <http://www.solism.ca>, All rights reserved.

 sKCF is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 **************************************************************************************************
 **************************************************************************************************/
#ifndef __trackers__SKCFDCF__
#define __trackers__SKCFDCF__

#include "tracker.h"
#include "utils.h"
#include "ktrackers.h"
#include <sstream>


class SKCFDCF :	public Tracker
{
private:
    
    KTrackers kcf;
    

public:
    SKCFDCF(KType type = KType::GAUSSIAN,
            KFeat feat = KFeat::FHOG ,
            bool scale = false, bool rotation=false): kcf(type, feat, scale, rotation){}
    
	~SKCFDCF(void)
	{
	}

	void setArea(const RotatedRect &rect)
    {
        kcf.setArea(rect);
    }
	
	
    
    //@Override
    void initialize(const cv::Mat &image,
                    const cv::Rect &rect)
    {
        float width =rect.width;
        float height=rect.height;
        Point2f center = Point2f(rect.tl().x + width/2.0,
                                 rect.tl().y + height/2.0);
        //update target size and location
        Size2f targetSize(width, height);
        kcf.setArea(RotatedRect(center,targetSize,0));
        kcf.processFrame(image);
    }

	void initialize(const cv::Mat &image,
		const cv::Rect &rect,vector<Point2f> &pts)
	{
		float length01 = sqrtf((pts[0].x - pts[1].x)*(pts[0].x - pts[1].x) + (pts[0].y - pts[1].y)*(pts[0].y - pts[1].y));
		float length12 = sqrtf((pts[1].x - pts[2].x)*(pts[1].x - pts[2].x) + (pts[1].y - pts[2].y)*(pts[1].y - pts[2].y));
		float width, height,angle;
		if (length01 > length12)
		{
			width = length01;
			height = length12;
			angle = atan2f(pts[0].y - pts[1].y, pts[0].x - pts[1].x);
		}
		else
		{
			width = length12;
			height = length01;
			angle = atan2f(pts[1].y - pts[2].y, pts[1].x - pts[2].x);
		}
		angle = angle * 180 / CV_PI;
		Point2f center = Point2f(rect.tl().x + rect.width / 2.0,
			rect.tl().y + rect.height / 2.0);
		Size2f targetSize(width,height);
		RotatedRect rotateRect(center,targetSize,angle);
		
		kcf.setArea(rotateRect);
		kcf.processFrame(image);
	}
    

  
    //@Override
    string virtual getDescription()
    {
        return "Andrés Solís Montero. sKCF: Scalable Kernel Correlation Filter with Sparse Feature Integration. 2015";
    }
    
    
   
    
    //@Override
    void getTrackedPoints(vector<Point2f> &pts)
    {
       // kcf.getTrackedPoints(pts);
    }
    
    //@Override
    void virtual getTrackedArea(vector<Point2f> &pts)
    {
        kcf.getTrackedArea(pts);
        
    }
    
    //@Override
    void processFrame(const cv::Mat &frame)
    {
        kcf.processFrame(frame);
    }
	//@Override
	double virtual getAngle()
	{
		return kcf.getAngle();
	}

};
#endif

#include "prak5.h"

prak5::prak5()
{

}

void prak5::findFeatures(cv::Mat src, cv::Mat processed)
{
    //convert to grayscale
    cv::cvtColor(src, src, CV_RGB2GRAY);
    cv::cvtColor(processed, processed, CV_RGB2GRAY);

    //step 1: detect the keypoints using SURF Detector, compute the descriptors
    cv::Ptr<cv::xfeatures2d::SURF> SURFdetector = cv::xfeatures2d::SURF::create();
    SURFdetector->setHessianThreshold(minHessian);

    std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
    cv::Mat descriptors_1, descriptors_2;

    cv::Mat mask;
    SURFdetector->detectAndCompute(src, mask, keypoints_1, descriptors_1);
    SURFdetector->detectAndCompute(processed, mask, keypoints_2, descriptors_2);

    //step 2: matching descriptor vectors using FLANN matcher
    cv::FlannBasedMatcher matcher;
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors_1, descriptors_2, matches);

    double max_dist = 0;
    double min_dist = 100;

    //quick calculation of max and min distances between keypoints
    for(size_t i=0; i<static_cast<unsigned int>(descriptors_1.rows); i++){
        double dist = static_cast<double>(matches[i].distance);
        if(dist < min_dist)
            min_dist = dist;
        if(dist > max_dist)
            max_dist = dist;
    }

    std::cout << "-- Max dist : " << max_dist << std::endl;
    std::cout << "-- Min dist : " << min_dist << std::endl;

    /* sort good_matches */
    good_matches.clear();   //clear good_matches

    for(size_t i=0; i<static_cast<unsigned int>(descriptors_1.rows); i++){
        if(static_cast<double>(matches[i].distance) <= cv::max(2*min_dist, minDistance)){
            good_matches.push_back(matches[i]);
        }
    }

    // draw only "good" matches
    cv::Mat img_matches;
    cv::drawMatches(src, keypoints_1, processed, keypoints_2,
                    good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
                    std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    //show detected matches
    cv::cvtColor(img_matches, img_matches, CV_BGR2RGB);
    cv::imshow("Good Matches", img_matches);

    for(size_t i=0; i < good_matches.size(); i++){
        std::cout << "Good Match [" << i << "] Keypoint 1: " << good_matches[i].queryIdx
                  << " -- Keypoint 2: " << good_matches[i].trainIdx << " -- Distance: " << good_matches[i].distance << std::endl;

        //in std::vector<Point2f> für getAffineTransform speichern
        points1.push_back(keypoints_1.at(good_matches[i].queryIdx).pt);
        points2.push_back(keypoints_2.at(good_matches[i].trainIdx).pt);
    }
}

void prak5::setMinHessian(double value){
    minHessian = value;
}

void prak5::setMinDistance(double value){
    minDistance = value;
}

void prak5::getTransformation(cv::Mat src, cv::Mat processed){
    if(points1.empty()){
        findFeatures(src, processed);
    }
    affineTransform = cv::getAffineTransform(points1, points2);
    //std::cout << affineTransform.rows << " " << affineTransform.cols << std::endl;
    std::cout << "affineTransform = " << std::endl << " " << affineTransform << std::endl << std::endl;
}

cv::Mat prak5::calcOriginal(cv::Mat src, cv::Mat processed){
    if(affineTransform.empty()){
        getTransformation(src, processed);
    }
    cv::Mat calc;
    cv::warpAffine(processed, calc, affineTransform, src.size(), CV_WARP_INVERSE_MAP);
    return calc;
}

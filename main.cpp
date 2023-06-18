#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>

void make_motion_images(const std::string& input_video, const std::string& output_image, int frame_step, int threshold) {
    // Load the video
    cv::VideoCapture video(input_video);

    std::vector<cv::Mat> frames;
    int frame_count = 0;
    while (true) {
        cv::Mat frame;
        bool ret = video.read(frame);
        if (ret) {
            if (frame_count % frame_step == 0) {
                cvtColor(frame, frame, cv::COLOR_BGR2HSV);
                frame.convertTo(frame, CV_32F);
                frames.push_back(frame);
            }
            frame_count += 1;
        }
        else {
            break;
        }
    }

    cv::Mat base_image = cv::imread(output_image);
    cvtColor(base_image, base_image, cv::COLOR_BGR2HSV);
    base_image.convertTo(base_image, CV_32F);

    cv::Mat resultImage = base_image.clone();
    // 各フレームのサイズをチェック
    const cv::Size& frameSize = frames[0].size();
    for (const cv::Mat& frame : frames) {
        if (frame.size() != frameSize) {
            throw std::invalid_argument("Frames have different sizes.");
        }
    }

    for (size_t i = 0; i < frames.size(); ++i) {
        cv::Mat diff;
        cv::absdiff(frames[i], base_image, diff);
        // 行数
        std::cout << "diff rows: " << diff.rows <<std::endl;
        // 列数
        std::cout << "diff cols: " << diff.cols << std::endl;

        

        //ここをセットにする
        cv::Mat mask = diff > threshold;
        // // 行数
        // std::cout << "mask rows: " << mask.rows <<std::endl;
        // // 列数
        // std::cout << "mask cols: " << mask.cols << std::endl;
        //std::cout << "mask: " << mask << std::endl;
        // cv::imwrite("mask_before" + std::to_string(i) + ".png", mask);

        // cv::Mat mask = base_image.clone();
        // // 行数
        // std::cout << "mask rows: " << mask.rows <<std::endl;
        // // 列数
        // std::cout << "mask cols: " << mask.cols << std::endl;

        // for (int r = 0; r < frameSize.height; ++r) {
        //     for (int c = 0; c < frameSize.width * 3; ++c) {
        //         // h,s,vのhとvを足してしきい値こえているかどうか。
        //         // if(c % 3 == 0) {
        //             int cc = c + 1;
        //             int ccc = c + 2;
        //             if((diff.at<float>(r, c) + diff.at<float>(r, ccc)) > threshold) {
        //                 mask.at<float>(r, c) = frames[i].at<float>(r, c);
        //                 // mask.at<float>(r, cc) = frames[i].at<float>(r, cc);
        //                 // mask.at<float>(r, ccc) = frames[i].at<float>(r, ccc);
        //             } else {
        //                 // mask.at<float>(r, c) = base_image.at<float>(r, c);
        //                 // mask.at<float>(r, cc) = base_image.at<float>(r, cc);
        //                 // mask.at<float>(r, ccc) = base_image.at<float>(r, ccc);
        //             }

        //         // }

        //     }
        // }

        // 行数
        std::cout << "mask rows: " << mask.rows <<std::endl;
        // 列数
        std::cout << "mask cols: " << mask.cols << std::endl;

        cv::imwrite("mask_after" + std::to_string(i) + ".png", mask);


        // resultImageに差分を一コマずつ足していく
        frames[i].copyTo(resultImage, mask);
        cv::Mat tempOverlay = resultImage.clone();
        tempOverlay.convertTo(tempOverlay, CV_8UC3);
        cvtColor(tempOverlay, tempOverlay, cv::COLOR_HSV2BGR);
        cv::imwrite("overlay_" + std::to_string(i) + ".png", tempOverlay);
    }

    resultImage.convertTo(resultImage, CV_8UC3);
    cvtColor(resultImage, resultImage, cv::COLOR_HSV2BGR);
    cv::imwrite("result_image.png", resultImage);

    // Release the video file pointer
    video.release();

    // Close all OpenCV windows
    cv::destroyAllWindows();
}




int main() {
    
    make_motion_images("sample.mp4", "base_image.png", 30, 30);
    return 0;


}

//cd build    
//cmake .. .
//make
//cd ..
//./build/main

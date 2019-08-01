#include <opencv2/highgui.hpp>
#include <list> //No matter
#include <cmath> //abs()

void testImage(const std::string WindowName,\
               cv::Mat TestingImage,\
               bool DestroyOrNot = true)
{
    //Show an image on window "winname", then destroy the window when pressing a key.
    cv::namedWindow(WindowName); //Create an window
    cv::imshow(WindowName, TestingImage); //Show the image on the window
    cv::waitKey(); //Waiting for pressing a key
    
    if (DestroyOrNot)
        cv::destroyWindow(WindowName); //Destroy the window
}

//bullshxtMount additionrea, cv::Scalar(0,0,255/*rand() & 255, rand() & 255, rand() & 255*/), 2);
//end bullshxtMount

void cutPic(cv::Mat & ToBeCut,\
            cv::Mat & Destination,\
            int thresholdCol=15,\
            int thresholdRow=15)
{
    std::list<int>memcol;
    std::list<int>memrow;
    int col=ToBeCut.cols, row=ToBeCut.rows;
    
    for (int i_cols = 0 ; i_cols < col-8 ; i_cols+=4)
    {
        double avg_b=0, avg_g=0, avg_r=0;
        //double avg_b_next=0, avg_g_next=0, avg_r_next=0;
        for (int i_rows = 0 ; i_rows < row ; i_rows++)
        {
            avg_b += ToBeCut.at<cv::Vec3b>(i_rows, i_cols)[0];
            avg_b -= ToBeCut.at<cv::Vec3b>(i_rows, i_cols+7)[0];
            avg_g += ToBeCut.at<cv::Vec3b>(i_rows, i_cols)[1];
            avg_g -= ToBeCut.at<cv::Vec3b>(i_rows, i_cols+7)[1];
            avg_r += ToBeCut.at<cv::Vec3b>(i_rows, i_cols)[2];
            avg_r -= ToBeCut.at<cv::Vec3b>(i_rows, i_cols+7)[2];
            
        }
        
        avg_b = std::abs(avg_b / row);
        avg_g = std::abs(avg_g / row);
        avg_r = std::abs(avg_r / row);
        
        //std::cout << avg_b << " " << avg_g << " " << avg_r << std::endl;
        if (avg_b >= 30 || avg_g >= 30 || avg_r >= 30)
        {
            memcol.push_back(i_cols);
            std::cout << avg_b << "\t" << avg_g << "\t" << avg_r << "\tcolumn:" << i_cols << std::endl;
            //cv::line(ToBeCut, cv::Point(i_cols,0), cv::Point(i_cols,row), cvScalar(255,255,255), 5, cv::LINE_8, 0);
        }
        else if ((avg_b >=thresholdCol && avg_g >= thresholdCol) ||\
                (avg_g >= thresholdCol && avg_r >= thresholdCol) ||\
                (avg_b >= thresholdCol && avg_r >= thresholdCol  ))
        {
            memcol.push_back(i_cols);
            std::cout << avg_b << "\t" << avg_g << "\t" << avg_r << "\tcolumn:" << i_cols << std::endl;
            //cv::line(ToBeCut, cv::Point(i_cols,0), cv::Point(i_cols,row), cvScalar(255,255,255), 5, cv::LINE_8, 0);
        }
        
    }
    
    for (int i_rows = 0 ; i_rows < row-8 ; i_rows+=4)
    {
        double avg_b=0, avg_g=0, avg_r=0;
        //double avg_b_next=0, avg_g_next=0, avg_r_next=0;
        for (int i_cols = 0 ; i_cols < col ; i_cols++)
        {
            avg_b += ToBeCut.at<cv::Vec3b>(i_rows, i_cols)[0];
            avg_b -= ToBeCut.at<cv::Vec3b>(i_rows, i_cols+7)[0];
            avg_g += ToBeCut.at<cv::Vec3b>(i_rows, i_cols)[1];
            avg_g -= ToBeCut.at<cv::Vec3b>(i_rows, i_cols+7)[1];
            avg_r += ToBeCut.at<cv::Vec3b>(i_rows, i_cols)[2];
            avg_r -= ToBeCut.at<cv::Vec3b>(i_rows, i_cols+7)[2];
            
        }
        
        avg_b = std::abs(avg_b / col);
        avg_g = std::abs(avg_g / col);
        avg_r = std::abs(avg_r / col);
        
        //std::cout << avg_b << " " << avg_g << " " << avg_r << std::endl;
        if (avg_b >= 30 || avg_g >= 30 || avg_r >= 30)
         {
             memrow.push_back(i_rows);
             std::cout << avg_b << "\t" << avg_g << "\t" << avg_r << "\trow:" << i_rows << std::endl;
             cv::line(ToBeCut, cv::Point(i_rows,0), cv::Point(i_rows,row), cvScalar(255,255,255), 5, cv::LINE_8, 0);
         }
         else if ((avg_b >=thresholdRow && avg_g >= thresholdRow) ||\
                    (avg_g >= thresholdRow && avg_r >= thresholdRow) ||\
                    (avg_b >= thresholdRow && avg_r >= thresholdRow))
         {
             memrow.push_back(i_rows);
             std::cout << avg_b << "\t" << avg_g << "\t" << avg_r << "\trow:" << i_rows << std::endl;
             cv::line(ToBeCut, cv::Point(i_rows,0), cv::Point(i_rows,row), cvScalar(255,255,255), 5, cv::LINE_8, 0);
         }
        
    }
    
    if (memcol.empty())
    {
        memcol.push_back(0);
        memcol.push_back(col);
    }
    else if (memcol.size() == 1)
    {
        memcol.pop_back();
        memcol.push_back(0);
        memcol.push_back(col);
    }
    
    if (memrow.empty())
    {
        memrow.push_back(0);
        memrow.push_back(row);
    }
    else if (memrow.size() == 1)
    {
        memrow.pop_back();
        memrow.push_back(0);
        memrow.push_back(col);
    }
    
    cv::Rect m_select = cv::Rect(memcol.front(), memrow.front(), memcol.back()-memcol.front(), memrow.back()-memrow.front());
    
    //testImage("shabby 0", ToBeCut);
    
    Destination = ToBeCut(m_select);
    
    testImage("shabby", Destination);
}
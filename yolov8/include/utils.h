#pragma once
#include <opencv2/opencv.hpp>
#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif
#include <fstream>  

static inline cv::Mat preprocess_img(cv::Mat& img, int input_w, int input_h) {
    int w, h, x, y;
    float r_w = input_w / (img.cols*1.0);
    float r_h = input_h / (img.rows*1.0);
    if (r_h > r_w) {
        w = input_w;
        h = r_w * img.rows;
        x = 0;
        y = (input_h - h) / 2;
    } else {
        w = r_h * img.cols;
        h = input_h;
        x = (input_w - w) / 2;
        y = 0;
    }
    cv::Mat re(h, w, CV_8UC3);
    cv::resize(img, re, re.size(), 0, 0, cv::INTER_LINEAR);
    cv::Mat out(input_h, input_w, CV_8UC3, cv::Scalar(128, 128, 128));
    re.copyTo(out(cv::Rect(x, y, re.cols, re.rows)));
    return out;
}

/**
 * 读取指定目录下的所有文件名
 * 
 * @param p_dir_name 目录路径
 * @param file_names 用于存储文件名的向量
 * @return 成功返回0，失败返回-1
 *
 * 此函数根据不同的操作系统平台，使用不同的方法来读取目录下的文件名
 * 在Windows平台上，使用FindFirstFile和FindNextFile函数来遍历目录
 * 在Linux平台上，使用opendir和readdir函数来遍历目录
 * 两个平台上的代码都忽略了"."和".."这两个特殊目录项
 */
static inline int read_files_in_dir(const char* p_dir_name, std::vector<std::string>& file_names)
{
#ifdef _WIN32
    // Windows平台的文件目录读取
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(std::string(std::string(p_dir_name) + "\\*").c_str(), &findFileData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        return -1;
    }

    do {
        if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
            file_names.push_back(findFileData.cFileName);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
    
    FindClose(hFind);
#else
    // Linux平台的文件目录读取
    DIR *p_dir = opendir(p_dir_name);
    if (p_dir == nullptr) {
        return -1;
    }

    struct dirent* p_file = nullptr;
    while ((p_file = readdir(p_dir)) != nullptr) {
        if (strcmp(p_file->d_name, ".") != 0 &&
            strcmp(p_file->d_name, "..") != 0) {
            std::string cur_file_name(p_file->d_name);
            file_names.push_back(cur_file_name);
        }
    }

    closedir(p_dir);
#endif

    return 0;
}

// Function to trim leading and trailing whitespace from a string
static inline std::string trim_leading_whitespace(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Src: https://stackoverflow.com/questions/16605967
static inline std::string to_string_with_precision(const float a_value, const int n = 2) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

static inline int read_labels(const std::string labels_filename, std::unordered_map<int, std::string>& labels_map) {
    std::ifstream file(labels_filename);
    // Read each line of the file
    std::string line;
    int index = 0;
    while (std::getline(file, line)) {
        // Strip the line of any leading or trailing whitespace
        line = trim_leading_whitespace(line);

        // Add the stripped line to the labels_map, using the loop index as the key
        labels_map[index] = line;
        index++;
    }
    // Close the file
    file.close();

    return 0;
}


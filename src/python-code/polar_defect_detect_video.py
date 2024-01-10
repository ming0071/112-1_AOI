import cv2
import numpy as np
import math

def nothing(x):
    pass

# 极坐标转换函数
def polar_transform_2(circles, frame_copy, roi_scale_factor):
    transformed_images = []
    for i in range(len(circles)):
        X = int(np.round(circles[i][0]))
        Y = int(np.round(circles[i][1]))
        R = int(np.round(circles[i][2]))
        new_R = int(R * roi_scale_factor)

        # 確保 ROI 坐標在圖像範圍內
        x1, y1 = max(X - new_R, 0), max(Y - new_R, 0)
        x2, y2 = min(X + new_R, frame_copy.shape[1]), min(Y + new_R, frame_copy.shape[0])

        # 檢查截取的 ROI 是否有效
        if x2 > x1 and y2 > y1:
            ROI = frame_copy[y1:y2, x1:x2]
            trans_center = (new_R, new_R)
            dst = cv2.warpPolar(ROI, (300, 600), trans_center, new_R, cv2.INTER_LINEAR + cv2.WARP_POLAR_LINEAR)
            transformed_images.append(dst)
        else:
            print(f"Invalid ROI for circle {i}")
            transformed_images.append(None)  # 或者使用其他的占位符

    return transformed_images


#缺陷檢測
def defect_detect(polar_img):
    transformed_images = []
    total_contour_area_list = []
    
    for img in polar_img:
        # 確保圖像是單通道的，如果不是，轉換為灰度圖
        if len(img.shape) == 3:
            img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            
        # 對每個圖像進行模糊
        blurred_roi = cv2.blur(img, (3, 501))
        # 對當前ROI區域和模糊後的ROI區域進行差分操作
        diff_roi = cv2.absdiff(img, blurred_roi)
        # 進行二值化操作
        _, binary_roi = cv2.threshold(diff_roi, threshold_value_for_diff, 255, cv2.THRESH_BINARY)
        # 添加二值化後的圖像到列表中
        transformed_images.append(binary_roi)
        # 如果需要尋找輪廓，可以在這裡添加對應的代碼
        contours, _ = cv2.findContours(binary_roi, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        # 計算每個輪廓的面積並累加到總和中
        total_contour_area = sum(cv2.contourArea(contour) for contour in contours)
        if total_contour_area > 300:
            total_contour_area_list.append(False)
        else:
            total_contour_area_list.append(True)
    print(total_contour_area_list)
    return transformed_images,total_contour_area_list

# 設定一個閾值，用於決定兩個圓形是否在垂直方向上足夠接近
y_threshold = 30  # 這個值可以根據您的具體需求進行調整

def sort_circles(circles):
    # 首先按 y 坐標排序
    sorted_circles = sorted(circles, key=lambda c: c[1])

    # 現在處理那些在垂直方向上足夠接近的圓形
    for i in range(len(sorted_circles) - 1):
        if abs(sorted_circles[i][1] - sorted_circles[i + 1][1]) < y_threshold:
            # 如果兩個圓形在 y 方向上足夠接近，則根據 x 坐標進行排序
            if sorted_circles[i][0] > sorted_circles[i + 1][0]:
                sorted_circles[i], sorted_circles[i + 1] = sorted_circles[i + 1], sorted_circles[i]

    return sorted_circles

# 检测圆形的函数
def detect_circles(minDist, param1, param2, threshold_value):
    global frame
    frame_copy = frame.copy()
    #--------------------------------------------------------
    gray = cv2.cvtColor(frame_copy, cv2.COLOR_BGR2GRAY)
    #--------------------------------------------------------
    # 指定中值濾波的核大小，通常為奇數
    kernel_size = 3
    median_blur_frame = cv2.medianBlur(gray, kernel_size)
    #--------------------------------------------------------
    _, thresholded = cv2.threshold(median_blur_frame, threshold_value, 255, cv2.THRESH_BINARY)
    #--------------------------------------------------------
    # 定義開運算的核（可以根據需求調整核的大小）
    kernel = np.ones((3, 3), np.uint8)
    # 使用開運算處理圖像
    opening = cv2.morphologyEx(thresholded, cv2.MORPH_OPEN, kernel)
    #--------------------------------------------------------
    contours, _ = cv2.findContours(opening, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    # 找到最大面积的轮廓
    max_contour = max(contours, key=cv2.contourArea)

    # 创建一个空白图像
    result = np.zeros_like(frame)

    # 绘制除了最大面积轮廓以外的其他轮廓
    for contour in contours:
        if contour is not max_contour:
            cv2.drawContours(result, [contour], 0, 255, 1)

    # 显示结果图像
    cv2.imshow("Result Image", result)
    result = cv2.cvtColor(result, cv2.COLOR_BGR2GRAY)
    #--------------------------------------------------------
    circles = cv2.HoughCircles(result, cv2.HOUGH_GRADIENT, dp=1, minDist=minDist,
                               param1=param1, param2=param2, minRadius=minRadius, maxRadius=maxRadius)
    if circles is not None:
        # 将圆形按照 x 和 y 坐标排序
        circles = np.round(circles[0, :]).astype("int")
        circles_sorted = sort_circles(circles)
        # 极坐标转换
        polar_img = polar_transform_2(circles_sorted,frame,(1+roi_scale_factor/10))
        diff_binary,area= defect_detect(polar_img)
        for i, (x, y, r) in enumerate(circles_sorted):
            # 绘制检测到的圆形
            cv2.circle(frame_copy, (x, y), r, (0, 255, 0), 4)
            cv2.circle(frame_copy, (x, y), 5, (0, 255, 0), -1)
            text = f"Circle {i}:{area[i]}"
            if area[i] == True:
                cv2.putText(frame_copy, text, (x - 50, y - 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
            else:
                cv2.putText(frame_copy, text, (x - 50, y - 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
            
            cv2.imshow(f"Polar Transform Circle {i}", polar_img[i])
            cv2.resizeWindow(f"Polar Transform Circle {i}", 800, 600)
            cv2.imshow(f"diff {i}",diff_binary[i])
            

        # 显示结果
        cv2.imshow("Detected Circles", frame_copy)
        cv2.imshow("Thresholded", thresholded)

    else:
        print("未检测到圆形")
        cv2.imshow("Thresholded", thresholded)
        cv2.imshow("Detected Circles", frame_copy)
    #--------------------------------------------------------    


# 读取
cap = cv2.VideoCapture(".//data//video//0105.avi")
# 获取视频的帧率
fps = cap.get(cv2.CAP_PROP_FPS)
# 计算每秒的帧数
frame_interval = int(fps)  # 每秒检测一次，需要读取的帧间隔

# 初始化帧计数器
frame_count = 0

# 初始參數
minDist = 115
param1 = 60
param2 = 24
threshold_value = 152
erosion_iterations = 1
minRadius = 1
maxRadius = 200
roi_scale_factor = 1
threshold_value_for_diff = 80
# 建立trakbar
cv2.namedWindow('Detected Circles')
cv2.createTrackbar('minDist', 'Detected Circles', minDist, 200, nothing)
cv2.createTrackbar('param1', 'Detected Circles', param1, 1000, nothing)
cv2.createTrackbar('param2', 'Detected Circles', param2, 1000, nothing)
cv2.createTrackbar('threshold_value', 'Detected Circles', threshold_value, 255, nothing)
cv2.createTrackbar('minRadius', 'Detected Circles', minRadius, 100, nothing)
cv2.createTrackbar('maxRadius', 'Detected Circles', maxRadius, 200, nothing)
cv2.createTrackbar('roi_scale_factor', 'Detected Circles', roi_scale_factor, 10, nothing)
cv2.createTrackbar('threshold_value_for_diff', 'Detected Circles', threshold_value_for_diff, 255, nothing)

while True:
    ret,frame = cap.read()
    minDist = cv2.getTrackbarPos('minDist', 'Detected Circles')
    param1 = cv2.getTrackbarPos('param1', 'Detected Circles')
    param2 = cv2.getTrackbarPos('param2', 'Detected Circles')
    threshold_value = cv2.getTrackbarPos('threshold_value', 'Detected Circles')
    minRadius = cv2.getTrackbarPos('minRadius', 'Detected Circles')
    maxRadius = cv2.getTrackbarPos('maxRadius', 'Detected Circles')
    roi_scale_factor = cv2.getTrackbarPos('roi_scale_factor', 'Detected Circles')
    threshold_value_for_diff = cv2.getTrackbarPos('threshold_value_for_diff', 'Detected Circles')
     # 每秒检测一次
    if frame_count % frame_interval == 0:
        detect_circles(minDist, param1, param2, threshold_value)

    frame_count += 1

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()

import cv2
import numpy as np

def nothing(x):
    pass

# Callback 函數，當軌跡條值改變時被調用
def detect_circles(minDist, param1, param2, threshold_value):
    global image, selected_circle
    img_copy = image.copy()  # 複製原始圖像以便每次重新繪製圓形

    gray = cv2.cvtColor(img_copy, cv2.COLOR_BGR2GRAY)

    # 二值化處理
    _, thresholded = cv2.threshold(gray, threshold_value, 255, cv2.THRESH_BINARY)
    # # 圖像侵蝕操作
    # kernel = np.ones((2, 2), np.uint8)
    # thresholded = cv2.erode(thresholded, kernel, iterations=erosion_iterations)

    # 偵測圓形
    circles = cv2.HoughCircles(thresholded, cv2.HOUGH_GRADIENT, dp=1, minDist=minDist,
                                param1=param1, param2=param2, minRadius=0, maxRadius=73)

    # 如果有偵測到圓形
    if circles is not None:
        circles = np.round(circles[0, :]).astype("int")

        for i, (x, y, r) in enumerate(circles):
            diameter = 2 * r
            print(f"圓形{i}直徑: {diameter}")

            # 繪製偵測到的所有圓形
            cv2.circle(img_copy, (x, y), r, (0, 255, 0), 4)
            cv2.circle(img_copy, (x, y), 5, (0, 255, 0), -1)  # 繪製圓心

            # 繪製方框（用于選擇圓）
            if selected_circle == i:
                expanded_radius = int(r * 1.2)  # 将圆的半径扩大为当前半径的1.1倍
                cv2.rectangle(img_copy, (x - expanded_radius, y - expanded_radius), (x + expanded_radius, y + expanded_radius), (255, 0, 0), 2)

        max_circles = len(circles)
        cv2.setTrackbarMax('selected_circle', 'Detected Circles', max_circles - 1)  # 設置最大值

        if selected_circle < max_circles:
            (x, y, r) = circles[selected_circle]
            diameter = 2 * r
            print(f"選擇的圓形直徑: {diameter}")

            # 創建一個與原始圖像尺寸相同的全黑遮罩
            mask = np.zeros(image.shape[:2], dtype=np.uint8)

            # 在遮罩上創建圓形（白色）
            cv2.circle(mask, (x, y), int(r*1.2), (255, 255, 255), thickness=-1)  # 用白色繪製選定的圓形區域

            # 使用bitwise_and將遮罩應用到原始圖像上
            selected_circle_region = cv2.bitwise_and(image, image, mask=mask)

            # 顯示選定的圓形區域
            cv2.imshow("Selected Circle Region", selected_circle_region)
            cv2.imshow("Detected Circles", img_copy)
            cv2.imshow("Origin", image)
            cv2.imwrite("edges.jpg", selected_circle_region)

    else:
        print("未偵測到圓形")
        cv2.imshow("thresholded", thresholded)  # 顯示二值化後的圖像

# 其他部分保持不變

# 读取相机校正参数
fs = cv2.FileStorage('calibration.xml', cv2.FILE_STORAGE_READ)
cameraMatrix = fs.getNode('cameraMatrix').mat()
distCoeffs = fs.getNode('distCoeffs').mat()
fs.release()
pixel_to_mm = 25.8 / 146
# 读取影像
image = cv2.imread('extracted_frame.jpg')

# 校正影像
image = cv2.undistort(image, cameraMatrix, distCoeffs)

minDist = 104  # 初始最小距离
param1 = 200   # 初始 param1
param2 = 10   # 初始 param2
threshold_value = 173  # 初始二值化阈值
erosion_iterations = 1  # 图像侵蚀的迭代次数
selected_circle = 0  # 选择的圆的索引（从0开始）

cv2.namedWindow('Detected Circles')
cv2.createTrackbar('minDist', 'Detected Circles', minDist, 200, nothing)
cv2.createTrackbar('param1', 'Detected Circles', param1, 1000, nothing)
cv2.createTrackbar('param2', 'Detected Circles', param2, 1000, nothing)
cv2.createTrackbar('threshold_value', 'Detected Circles', threshold_value, 255, nothing)
cv2.createTrackbar('selected_circle', 'Detected Circles', selected_circle, 10, nothing)

while True:
    minDist = cv2.getTrackbarPos('minDist', 'Detected Circles')
    param1 = cv2.getTrackbarPos('param1', 'Detected Circles')
    param2 = cv2.getTrackbarPos('param2', 'Detected Circles')
    threshold_value = cv2.getTrackbarPos('threshold_value', 'Detected Circles')
    selected_circle = cv2.getTrackbarPos('selected_circle', 'Detected Circles')

    detect_circles(minDist, param1, param2, threshold_value)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()

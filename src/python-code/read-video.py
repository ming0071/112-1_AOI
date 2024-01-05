import cv2

# 指定影片檔案的路徑
video_path = '.\\data\\video\\output.avi'  # 請替換為實際的檔案路徑

# 打開影片檔案
cap = cv2.VideoCapture(video_path)

# 確保影片檔案成功打開
if not cap.isOpened():
    print("Error: 無法打開影片檔案.")
    exit()

# 獲取影片的基本資訊
fps = int(cap.get(cv2.CAP_PROP_FPS))
width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

# 顯示影片的視窗
cv2.namedWindow('Video Player', cv2.WINDOW_NORMAL)
cv2.resizeWindow('Video Player', width, height)

while True:
    # 讀取一幀的影像
    ret, frame = cap.read()

    # 如果影片讀取完畢，則退出迴圈
    if not ret:
        break

    # 顯示當前影像
    cv2.imshow('Video Player', frame)

    # 按 'q' 鍵退出迴圈
    if cv2.waitKey(30) & 0xFF == ord('q'):
        break

# 釋放資源
cap.release()
cv2.destroyAllWindows()

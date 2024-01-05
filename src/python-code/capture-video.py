import cv2

# 打開相機
cap = cv2.VideoCapture(0)  # 0 代表默認相機

# 設定影片的解析度和幀率
frame_width = int(cap.get(3))
frame_height = int(cap.get(4))
out = cv2.VideoWriter('.\\data\\video\\webcam.avi', cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'), 10, (frame_width, frame_height))

while True:
    # 讀取一幀的影像
    ret, frame = cap.read()

    # 顯示當前影像
    cv2.imshow('Camera Feed', frame)

    # 將當前幀寫入影片
    out.write(frame)

    # 按 'q' 鍵退出迴圈
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 釋放資源
cap.release()
out.release()
cv2.destroyAllWindows()

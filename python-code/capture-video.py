import cv2

def main():
    # 初始化相機
    camera = cv2.VideoCapture(1)
    
    # 設定相機解析度為 720p
    camera.set(3, 800)  # 寬度
    camera.set(4, 600)   # 高度
    
    # 檢查相機是否成功初始化
    if not camera.isOpened():
        print("無法啟動相機")
        return
    
    # 視窗縮放比例
    scale = 1.0
    
    # 儲存相片的計數器
    photo_count = 0
    
    # 顯示視窗
    cv2.namedWindow('Camera')
    
    while True:
        # 從相機中讀取影像
        ret, frame = camera.read()
        
        # 檢查影像是否成功讀取
        if not ret:
            print("無法讀取影像")
            break
        
        # 保持原始解析度，調整視窗大小
        window_width = int(frame.shape[1] * scale)
        window_height = int(frame.shape[0] * scale)
        cv2.resizeWindow('Camera', window_width, window_height)
        
        # 顯示影像
        cv2.imshow('Camera', frame)
        
        # 按下空白鍵拍照並儲存影像
        key = cv2.waitKey(1)
        if key == ord(' '):
            photo_count += 1
            photo_name = f'photo_{photo_count}.jpg'
            cv2.imwrite(photo_name, frame)
            print(f'已儲存 {photo_name}')
            cv2.imshow('Photo', frame)
        
        # 按下 'A' 鍵縮小視窗
        elif key == ord('a'):
            scale = max(0.1, scale - 0.1)
        
        # 按下 'D' 鍵放大視窗
        elif key == ord('d'):
            scale = min(2.0, scale + 0.1)
        
        # 按下 'Q' 鍵關閉檢視視窗
        elif key == ord('q'):
            break
    
    # 釋放資源並關閉視窗
    camera.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()

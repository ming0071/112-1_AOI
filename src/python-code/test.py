import cv2
import numpy as np

# 創建一個 NumPy 陣列作為示例數據
width, height = 640, 480
num_frames = 100
frames = np.random.randint(0, 255, (num_frames, height, width, 3), dtype=np.uint8)

# 指定影片輸出的路徑和名稱
output_path = 'output_video.avi'

# 創建 VideoWriter 物件
fourcc = cv2.VideoWriter_fourcc(*'XVID')  # 可根據需要更改編碼方式
out = cv2.VideoWriter(output_path, fourcc, 20.0, (width, height))

# 將 NumPy 陣列中的每一幀寫入影片
for frame in frames:
    out.write(frame)

# 釋放資源
out.release()

print(f"影片已成功保存到 {output_path}")

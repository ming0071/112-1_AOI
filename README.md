# 112-1_AOI 圓管缺陷檢測期末專案

本專案目的是由工業相機對金屬圓管取像之後，再由 C++ 進行後續的影像處理，主要會對於影像處理的過程進行說明。<br>
以下是專案中的圓管的實際照片:<br><br>
![image](https://github.com/ming0071/112-1_AOI-final-project/blob/main/docs/tube.png)
程式流程圖:<br>
![image](https://github.com/ming0071/112-1_AOI-final-project/blob/main/docs/circle.png)

## 圖片預處理

第一步讀取經由工業相機拍攝之後的照片，後續經由中值濾波、二值化、開運算等基本影像處理，最後對這些結果的圖片體取出最外輪廓去除(即最大面積輪廓)，得到僅保留內圓輪廓的效果，確保後續的霍夫測圓可以只抓取欸圓的特徵做後續的影像辨識。
![image](https://github.com/ming0071/112-1_AOI-final-project/blob/main/docs/pre-process.png)

## 以極座標轉換做輪廓檢測

一開始先是由霍夫轉換得到每個圓的圓心座標、半徑，再對結果進行上到下、左而右的排序，藉由這樣的資訊抓取 ROI 之後就開始極座標輪廓檢測的過程。<br>
1. 極座標轉換
2. 均值濾波，以(3,501)的濾波核進行濾波，得到假想的完美圓，即沒有缺陷
3. 差值計算，以極座標轉換和假想圓做差值計算，這樣得到的結果就會是缺陷的特徵
4. 缺陷特徵二值化
5. 缺陷特徵輪廓抓取
6. 反極座標轉換
![image](https://github.com/ming0071/112-1_AOI-final-project/blob/main/docs/process.png)

而缺陷的判定就可以藉由計算每個圓內缺陷區域的像素面積而得，當大於設定的閥值即為 NG、小於設定的閥值即為 OK
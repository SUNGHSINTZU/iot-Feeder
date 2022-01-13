import cv2
import os
import datetime,time
import math

#%% define function
def savepic(frame):
    date = datetime.datetime.now().strftime("%m_%d_%Y_%H_%M_%S")
    img_name = CAMERA_NAME+"_{}.png".format(date)
    img_path = os.path.join(path, img_name)
    cv2.imwrite(img_path,frame)
    print("{}".format(img_name))

    return str(img_path)

def pellet_area_ratio(image_path):

    image = cv2.imread(image_path)
    # change the (y1,x1) and (y2,x2)
    (h, w) = image.shape[:2]
    x1 = math.ceil(w*0.1)
    y1 = math.ceil(h*0.5)
    x2 = math.ceil(w*0.7)
    y2 = math.ceil(h*0.9)
    
#     x1 = math.ceil(w*0)
#     y1 = math.ceil(h*0)
#     x2 = math.ceil(w*1)
#     y2 = math.ceil(h*1)
    mask = image[y1:y2, x1:x2]
    clone = image.copy()
    clone[y1:y2, x1:x2] = (255,153,51)

    AdaptiveThresholdTypes = cv2.ADAPTIVE_THRESH_MEAN_C
    # AdaptiveThresholdTypes = cv2.ADAPTIVE_THRESH_GAUSSIAN_C 
    thresholdType = cv2.THRESH_BINARY_INV
    RetrievalModes = cv2.RETR_TREE
    ContourApproximationModes= cv2.CHAIN_APPROX_NONE 

    # region of intrest
    gray = cv2.cvtColor(mask, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (9,9), 0.2)
    thresh = cv2.adaptiveThreshold(blurred,255, AdaptiveThresholdTypes, thresholdType,21,5)

    (cnts, _) = cv2.findContours(thresh.copy(), RetrievalModes, ContourApproximationModes)
    contour=cv2.drawContours(mask.copy(), cnts, -1, (0, 255, 0), 1)

    mask_pixels = cv2.countNonZero(thresh)
    mask_area = (x2-x1) * (y2-y1)
    print(h*w)
    print(mask_pixels)
    print(mask_area)
    area_ratio = (mask_pixels / mask_area) * 100
    area_ratio = "%.2f" % area_ratio
    area_ratio = "A-" + area_ratio
    print(area_ratio)

    # show image
    if camera_mode == "h":
        cv2.imshow("image",image)
        cv2.imshow("clone",clone)
        cv2.imshow("thresh",thresh)
        cv2.imshow("contour",contour)
        cv2.waitKey(0)

    return area_ratio


#%% main code
# 選擇攝影機
# cap = cv2.VideoCapture("http://192.168.0.111:81/stream")
cap = cv2.VideoCapture("http://192.168.50.110:81/stream")
CAMERA_NAME = 'CAM0'
PIC_PERIOD = 5.0
# path = 'C:\Gloria\College Study\\110-1\Introduction to the Internet of Things\project\\rawpic'
path='/home/pi/Desktop/iot-project/rawpic'
if not cap.isOpened():
    print("Cannot open camera")
    exit()


START_TIME = time.time()
print("start time: ",START_TIME)

# 拍照模式:自動或手動
camera_mode = input("camera mode [a(auto)/h(human)]? ")
if  camera_mode == "a": # auto mode
    print('auto mode')
    count = 0
    while(True):
        # 從攝影機擷取一張影像
        ret, frame = cap.read()
        if not ret:
            print("Can't receive frame (stream end?). Exiting ...")
            break

        # 每PIC_PERIOD秒拍一張照片
        delay = PIC_PERIOD - ((time.time() - START_TIME) % PIC_PERIOD)
        if delay > PIC_PERIOD-0.1:
            print("delay:",delay)
            print("count",count)
            dst = savepic(frame)
            ratio = pellet_area_ratio(dst)
            # aret = write_read(ratio)

            count += 1
        
        # 顯示圖片
        cv2.imshow(CAMERA_NAME, frame)

        if cv2.waitKey(1) == ord('q'):
            break

elif camera_mode == "h": # human mode
    print('human mode')
    while(True):
        # 從攝影機擷取一張影像
        ret, frame = cap.read()
        if not ret:
            print("Can't receive frame (stream end?). Exiting ...")
            break

        # 按空白鍵拍照
        if cv2.waitKey(1) == ord(' '): 
            dst = savepic(frame)
            ratio = pellet_area_ratio(dst)

        # 顯示圖片
        cv2.imshow(CAMERA_NAME, frame)

        if cv2.waitKey(1) == ord('q'):
            break
    
# 釋放攝影機
cap.release()

# 關閉所有 OpenCV 視窗
cv2.destroyAllWindows()

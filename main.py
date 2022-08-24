import cv2
import numpy as np
from collections import deque
from enum import Enum
import time
import pyautogui

# F_C = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
#
F_C = cv2.CascadeClassifier('haarcascade_eye.xml')


class Moves(Enum):
    LEFT = 0
    TOP = 1
    RIGHT = 2
    DOWN = 3

moves_queue = deque([0]*4, maxlen = 4)
last_vertical, last_horizontal = 0, 0

#
detector_params = cv2.SimpleBlobDetector_Params()
detector_params.filterByArea = True
detector_params.maxArea = 1500
detector = cv2.SimpleBlobDetector_create(detector_params)
#
idx = 0
cap = cv2.VideoCapture(idx)
key = 0
diameter=[]
blink =False
bcount = -1
kernel = np.ones((3,3),np.uint8)
# frame = cv2.imread("./steve.jpg",cv2.IMREAD_COLOR)

ACTION_DELAY = 500 # ms
last_action_time = time.perf_counter_ns()
while True:
    ret, frame = cap.read()
    G_scale = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    Eyes = F_C.detectMultiScale(G_scale, 1.1, 7)
    print(Eyes)
    if len(Eyes) > 0:
        a = "Eye Open"
        for (ex, ey, ew, eh) in Eyes:
            # cv2.rectangle(img,(ex,ey),(ex+ew,ey+eh),(0,255,0),2)
            roi_gray2 = G_scale[ey:ey + eh, ex:ex + ew]

            # roi_color2 = img[ey:ey + eh, ex:ex + ew]
            blur = cv2.GaussianBlur(roi_gray2, (5, 5), 10)
            ret3, th3 = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
            new_img = cv2.adaptiveThreshold(blur, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 53, 0)
            erosion = cv2.dilate(new_img, kernel, iterations=5)
            img = cv2.morphologyEx(erosion, cv2.MORPH_OPEN, kernel, iterations=4)
            img = cv2.GaussianBlur(img, (5, 5), 5)
            # img = cv2.morphologyEx(img, cv2.MORPH_CLOSE, kernel, iterations=8)
            img = cv2.dilate(img, None, iterations=3)  # 2
            img = cv2.morphologyEx(img, cv2.MORPH_CLOSE, kernel, iterations=3)
            img = cv2.morphologyEx(img, cv2.MORPH_OPEN, kernel, iterations=8)
            img = cv2.erode(img, None, iterations=4)  # 1

            keypoints = detector.detect(img)
            img = cv2.drawKeypoints(img, keypoints,img, (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)


            cv2.imshow("",img)
            circles = cv2.HoughCircles(th3,cv2.HOUGH_GRADIENT,4,200,param1=20,param2=150,minRadius=0,maxRadius=0)

            if keypoints is not None:

                # for i in keypoints[0, :]:
                #     print(i[0])
                    # if (i[2] > 0 and i[2] < 155):
                # keypoints = np.uint16(np.around(circles))
                cv2.drawKeypoints(img, keypoints,img, (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    # cv2.imshow('IMG', frame)
    if cv2.waitKey(1) == ord('q'):
        break
    # for (x, y, w, h) in Eyes:
    #     cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 3)
    #
    #     if ((time.perf_counter_ns() - last_action_time) // 1_000_000) > ACTION_DELAY:
    #         if last_vertical or last_horizontal:
    #             diff_vertical = y - last_vertical
    #             diff_horizontal = x - last_horizontal
    #
    #             if abs(diff_vertical) > abs(diff_horizontal):
    #                 # vertical movement
    #                 if diff_vertical < 0:
    #                     moves_queue.appendleft(Moves.TOP)
    #                 else:
    #                     moves_queue.appendleft(Moves.DOWN)
    #             else:
    #                 # horizontal movement
    #                 if diff_horizontal > 0:
    #                     moves_queue.appendleft(Moves.LEFT)
    #                 else:
    #                     moves_queue.appendleft(Moves.RIGHT)
    #
    #
    #     # save last pos
    #     last_vertical = y
    #     last_horizontal = x
    #
    #     #check for action
    #     move_made = False
    #     if all(m == Moves.LEFT for m in moves_queue):
    #         # LEFT
    #         print("LEFT")
    #         move_made = True
    #         pyautogui.keyDown('left')
    #         pyautogui.keyUp('left')
    #     elif all(m == Moves.TOP for m in moves_queue):
    #         # TOP
    #         print("TOP")
    #         move_made = True
    #         pyautogui.keyDown('up')
    #         pyautogui.keyUp('up')
    #     elif all(m == Moves.RIGHT for m in moves_queue):
    #         # RIGHT
    #         print("RIGHT")
    #         move_made = True
    #         pyautogui.keyDown('right')
    #         pyautogui.keyUp('right')
    #     elif all(m == Moves.DOWN for m in moves_queue):
    #         # DOWN
    #         print("DOWN")
    #         move_made = True
    #         pyautogui.keyDown('down')
    #         pyautogui.keyUp('down')
    #
    #     if move_made:
    #         last_action_time = time.perf_counter_ns()
    #         last_horizontal = 0
    #         last_vertical = 0
    #         moves_queue = deque([0]*4,maxlen=4)
    #
    #
    # cv2.imshow('IMG', frame)
    # # key = cv2.waitKey(1)
    # if cv2.waitKey(1) == ord('q'):
    #     break

cap.release()
cv2.destroyAllWindows()


# def blob(img,detector):



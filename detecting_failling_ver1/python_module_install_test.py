import cv2
import mediapipe as mp
from tkinter import *
from tkinter.filedialog import *
from PIL import Image, ImageTk  # Pillow 라이브러리 추가


# MediaPipe를 사용하여 관절 추적 설정
def detect_video_self():
    detect_video(False)


def detect_video_video():
    detect_video(True)


def detect_video(check=False):
    mp_pose = mp.solutions.pose
    pose = mp_pose.Pose()

    if check:
        file = askopenfile(parent=window, mode='rb', defaultextension='*.mp4',
                           filetypes=(('MP4 Files', '*.mp4'), ('All Files', '*.*')))
        if file is None:
            return
        video_path = file.name
        cap = cv2.VideoCapture(video_path)
    else:
        cap = cv2.VideoCapture(0)  # 실시간 웹캠 영상을 열기

    # 이전 프레임의 머리 y 좌표 초기화
    previous_head_y = None

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break

        # RGB로 변환
        image_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        # 관절 추적
        result = pose.process(image_rgb)

        # 관절 정보를 그리기
        if result.pose_landmarks:
            mp.solutions.drawing_utils.draw_landmarks(frame, result.pose_landmarks, mp_pose.POSE_CONNECTIONS)

            # 넘어짐 판단 로직
            landmarks = result.pose_landmarks.landmark
            head = landmarks[mp_pose.PoseLandmark.NOSE.value]
            right_ankle = landmarks[mp_pose.PoseLandmark.RIGHT_ANKLE.value]
            left_ankle = landmarks[mp_pose.PoseLandmark.LEFT_ANKLE.value]
            right_hand = landmarks[mp_pose.PoseLandmark.RIGHT_WRIST]
            left_hand = landmarks[mp_pose.PoseLandmark.LEFT_WRIST]

            # 조건 1: 머리의 높이가 발목 위치와 비슷해지거나 낮아졌을 때
            if head.y > right_ankle.y - 0.1 or head.y > left_ankle.y - 0.1:
                cv2.putText(frame, 'FALLING DETECT', (50, 200), cv2.FONT_ITALIC, 5, (0, 0, 255), 2, cv2.LINE_AA)

            # 조건 2: 머리의 y 좌표가 이전 프레임보다 급격하게 아래로 내려갔을 때
            if previous_head_y is not None:
                if previous_head_y - head.y > 0.2:  # y 좌표가 0.2 이상 급격히 내려갔을 때
                    cv2.putText(frame, 'FALLING DETECT', (50, 200), cv2.FONT_ITALIC, 5, (0, 0, 255), 2, cv2.LINE_AA)

            if right_hand.y > right_ankle.y - 0.05 or left_hand.y > left_ankle.y - 0.05:
                cv2.putText(frame, 'FALLING DETECT', (50, 200), cv2.FONT_ITALIC, 5, (0, 0, 255), 2, cv2.LINE_AA)

            # 현재 프레임의 머리 y 좌표를 저장
            previous_head_y = head.y

        # 화면에 결과 표시
        frame_resized = cv2.resize(frame, (800, 600))  # 원하는 크기로 조정
        cv2.imshow('Fall Detection', frame_resized)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()


# 메뉴 만들기
window = Tk()
window.geometry('512x512')
window.title('Color Image Processing (Preview 1)')

# 이미지 불러오기 및 라벨에 표시
img_path = 'bg_fallDetect.png'  # 표시할 이미지 파일 경로
img = Image.open(img_path)
img = img.resize((512, 512))  # 윈도우 크기에 맞게 이미지 크기 조정
img_tk = ImageTk.PhotoImage(img)

label = Label(window, image=img_tk)
label.place(x=0, y=0, relwidth=1, relheight=1)  # 이미지가 윈도우 전체를 덮도록 배치

mainMenu = Menu(window)
window.config(menu=mainMenu)

fileMenu = Menu(mainMenu, tearoff=0)
mainMenu.add_cascade(label='실시간 영상', menu=fileMenu)
fileMenu.add_command(label='실시간 영상', command=detect_video_self)

pixelMenu = Menu(mainMenu, tearoff=0)
mainMenu.add_cascade(label='영상 확인', menu=pixelMenu)
pixelMenu.add_command(label='영상 확인', command=detect_video_video)

window.mainloop()
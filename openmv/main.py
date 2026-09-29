import sensor, image, time, pyb
from pyb import UART

uart = UART(3, 115200, timeout_char=200)
uart.init(115200, bits=8, parity=None, stop=1)

black_threshold = (11, 45, -22, 5, -18, 18) # 黑色阈值
red_threshold = (16, 41, 14, 41, 4, 44) # 红色阈值
green_threshold = (37, 78, -36, -15, -9, 42)
# 滤波全局变量
filtered_x = 0.0
filtered_y = 0.0
first_frame = True

# 摄像头初始化
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(10)
# sensor.set_auto_exposure(False, exposure_us=50000)
# sensor.set_auto_whitebal(False)
clock = time.clock()


def adaptive_iir_filter(raw_x, raw_y):
    """自适应一阶IIR低通滤波"""
    global first_frame, filtered_x, filtered_y

    if first_frame:
        first_frame = False
        filtered_x = raw_x
        filtered_y = raw_y
        return filtered_x, filtered_y

    # 计算运动幅度
    move_mag = ((raw_x - filtered_x)**2 + (raw_y - filtered_y)**2)**0.5

    # 动态调整α
    if move_mag > 50:
        alpha = 0.5
    elif move_mag > 20:
        alpha = 0.4
    elif move_mag > 10:
        alpha = 0.3
    else:
        alpha = 0.2

    # 更新全局滤波值
    filtered_x = alpha * raw_x + (1 - alpha) * filtered_x
    filtered_y = alpha * raw_y + (1 - alpha) * filtered_y

    return filtered_x, filtered_y

def find_max(blobs):
    max_size = 0
    max_blob = None
    for blob in blobs:
        if blob.pixels() > max_size:
            max_blob = blob
            max_size = blob.pixels()
    return max_blob

def send_data(x, y, flag):
    """串口发送数据"""
    try:
        # 转为整数，减少数据量，MCU解析更方便
        data = f"{int(x)} {int(y)} {flag} "
        uart.write(data.encode())  # 字符串转字节流发送
    except Exception as e:
        print("串口错误：", e)

while True:
    clock.tick()
    img = sensor.snapshot()
    blobs = img.find_blobs([red_threshold],pixels_threshold=400)

    if blobs:
        max_blob = find_max(blobs)
        if max_blob is not None:
            cx = max_blob.cx()
            cy = max_blob.cy()
            #adaptive_iir_filter(cx, cy)
            flag = 1
            #原始坐标红叉，滤波后绿叉
            img.draw_rectangle(max_blob.rect())
            img.draw_cross(cx, cy, color=(255, 0, 0))
            #img.draw_cross(int(filtered_x), int(filtered_y), color=(0, 255, 0))
        else:
            # 无有效色块，重置
            flag = 0
            first_frame = True
            cx = 0.0
            cy = 0.0
    else:
        # 无色块，重置
        flag = 0
        first_frame = True
        cx = 0.0
        cy = 0.0
    print(clock.fps())
    # 发送滤波后的数据
    send_data(cx, cy, flag)


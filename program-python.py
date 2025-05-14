import torch
import cv2
import serial
import time

# Inisialisasi model YOLOv5 dengan weights yang sudah di-train
model = torch.hub.load('ultralytics/yolov5', 'custom', 
                        path=r"C:\Pemrograman\CAPSTONE-2\yolov5\runs\train\exp\weights\best.pt", 
                        source='local')

# Inisialisasi port serial (ganti dengan port yang sesuai di komputer Anda)
arduino = serial.Serial(port='COM4', baudrate=9600, timeout=2)
time.sleep(2)  # Tunggu beberapa detik untuk inisialisasi port serial

# Fungsi untuk mengirim data ke Arduino
def send_to_arduino(data): 
    try:
        arduino.write((data + "\n").encode())  # Kirim data ke Arduino
        arduino.flush()  # Pastikan data dikirim sepenuhnya
    except serial.SerialException as e:
        print(f"Error writing to serial port: {e}")

# URL stream
# url = 'http://192.168.1.16:81/stream'
cap = cv2.VideoCapture(1)
cap.set(cv2.CAP_PROP_READ_TIMEOUT_MSEC, 50000)  # Tambahkan timeout menjadi 50 detik


if not cap.isOpened():
    print("Error: Kamera tidak bisa diakses.")
    exit()

while True:
    # Baca frame dari kamera
    ret, frame = cap.read()
    if not ret:
        print("Error: Gagal membaca frame.")
        break

    # Deteksi objek menggunakan YOLOv5
    results = model(frame)

    # Mendapatkan hasil deteksi
    labels, coords = results.xyxyn[0][:, -1], results.xyxyn[0][:, :-1]

    # Tampilkan deteksi di frame
    for i, coord in enumerate(coords):
        if coord[4] >= 0.25:  # confidence threshold
            x1, y1, x2, y2 = int(coord[0] * frame.shape[1]), int(coord[1] * frame.shape[0]), int(coord[2] * frame.shape[1]), int(coord[3] * frame.shape[0])
            label = results.names[int(labels[i])]

            # Gambar kotak pembatas dan label
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)

            # Jika sampah terdeteksi, klasifikasi jenis sampah dan kirim data ke Arduino
            if label in ['PlasticBottle']:
                send_to_arduino('plastic')  # Kirim data ke Arduino
                print("Sampah plastik terdeteksi, memisahkan...")
                time.sleep(5)  # Tunggu sampai servo selesai bergerak
            elif label in ['Metal Can']:
                send_to_arduino('metal')  # Kirim data ke Arduino
                print("Sampah kaleng terdeteksi, memisahkan...")
                time.sleep(5)  # Tunggu sampai servo selesai bergerak

    # Tampilkan frame dengan deteksi
    cv2.imshow('Deteksi Sampah', frame)

    # Keluar dengan menekan 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Bersihkan resources
cap.release()
cv2.destroyAllWindows()
